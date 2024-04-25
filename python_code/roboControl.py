import math

def robo_move(err_x, err_y):
    if abs(err_x)>5:
        if err_x>0:
            return f'm0,-0.2,0;\n'
        elif err_x<0:
            return f'm0,0.2,0;\n'
    elif abs(err_y)>5:
        if err_x>0:
            return f'm-0.2,0,0;\n'
        elif err_x<0:
            return f'm0.2,0,0;\n'
    else:
        return None

def arm_rec():
    return 'p0,145,100o0,90,90;'

def arm_grab():
    return 'p0,-15,145o0,90,90g;'