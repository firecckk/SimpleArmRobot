from inference import get_model
import supervision as sv
import cv2
import time
import json
from mqtt import *
from roboControl import *

def count_FPS():
    # 计算帧率
    global frame_count
    global start_time
    frame_count += 1
    if frame_count >= 30:  # 每隔30帧计算一次帧率
        end_time = time.time()
        fps = frame_count / (end_time - start_time)
        print(f"FPS: {fps:.2f}")
        frame_count = 0
        start_time = end_time

###### MAIN ######
start_time = time.time()
frame_count = 0

# connect to mqtt
client = connect_mqtt()
client.loop_start()

# init arm
send_msg(client, arm_rec())
aim_point_x = 275
aim_point_y = 215

model = get_model(model_id="get-object-size/7", api_key="S2B330DSLZYRwtDzQqx5")

# MJPEG流的URL
stream_url = 'http://192.168.43.82:81/stream'

# 创建视频捕获对象
cap = cv2.VideoCapture(stream_url)

# 检查捕获对象是否成功打开
if not cap.isOpened():
    print("Error: Could not open MJPEG stream.")
    exit()

time_stamp=0
while True:
    count_FPS();
    # 从MJEPG流中读取一帧
    ret, frame = cap.read()

    # 检查是否成功读取帧
    if not ret:
        print("Error: Failed to read frame.")
        break

    #if flag:
    #    #flag = False
    #    results = model.infer(frame)
    #    # load the results into the supervision Detections api
    #    detections = sv.Detections.from_inference(results[0].dict(by_alias=True, exclude_none=True))
    #    # create supervision annotators
    #    bounding_box_annotator = sv.BoundingBoxAnnotator()
    #    label_annotator = sv.LabelAnnotator()
    #    # annotate the image with our inference results
    #    annotated_image = bounding_box_annotator.annotate(scene=frame, detections=detections)
    #    annotated_image = label_annotator.annotate(scene=annotated_image, detections=detections)
        
    results = model.infer(frame)
    # 循环处理每个结果
    for result in results:
        for prediction in result.predictions:
            class_name = prediction.class_name
            x, y, w, h = int(prediction.x), int(prediction.y), int(prediction.width), int(prediction.height)
            confidence = int(prediction.confidence);
            
            err_x, err_y = aim_point_x - x, aim_point_y - y
            
            # 在图像上画出边界框
            x1 = int(x - w/2)
            y1 = int(y - h/2)
            x2 = int(x + w/2)
            y2 = int(y + h/2)
            
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.line(frame, (x, y - 5), (x, y + 5), (255, 0, 0), 2)
            cv2.line(frame, (x - 5, y), (x + 5, y), (255, 0, 0), 2)

            # 添加类别标签
            text = f'{class_name} {confidence:.2f}'
            cv2.putText(frame, text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.3, (0, 255, 0), 2)
            text = f'{x} {y}'
            cv2.putText(frame, text, (x, y + 10), cv2.FONT_HERSHEY_SIMPLEX, 0.3, (0, 255, 0), 2)
            
            # 准星
            cv2.line(frame, (aim_point_x - 10, aim_point_y), (aim_point_x + 10, aim_point_y), (0, 0, 255), 2)
            cv2.line(frame, (aim_point_x, aim_point_y - 10), (aim_point_x, aim_point_y + 10), (0, 0, 255), 2)
            text = f'{err_x} {err_y}'
            
            if time.time() - time_stamp > 2:
                time_stamp = time.time()
                m = robo_move(err_x, err_y)
                if m is not None:
                    send_msg(client, m)
                else:
                    send_msg(client, arm_grab())
                    time.sleep(1)
                    send_msg(client, arm_rec())
            cv2.putText(frame, text, (aim_point_x, aim_point_y + 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
    cv2.imshow("frame", frame)
            
    # 按下 'q' 键退出循环
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    

# 释放资源并关闭窗口
cap.release()
client.loop_stop()
cv2.destroyAllWindows()

