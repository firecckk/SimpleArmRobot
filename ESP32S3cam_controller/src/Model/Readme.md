当末端执行器位于第一象限时：
$$
\begin{align*}
D &= \sqrt{X_{\text{End\_Effector}}^2 + Z_{\text{End\_Effector}}^2} \\
\Theta_1 &= \left(\arctan\left(\frac{Z_{\text{End\_Effector}}}{X_{\text{End\_Effector}}}\right)\right) \cdot \frac{180.00}{\pi} \\
d &= D - L4 \\
\text{Yoffset} &= Y_{\text{End\_Effector}} - L1 \\
R &= \sqrt{d^2 + \text{Yoffset}^2} \\
\alpha_1 &= \left(\arccos\left(\frac{d}{R}\right)\right) \cdot \frac{180.00}{\pi} \\
\alpha_2 &= \left(\arccos\left(\frac{L2^2 + R^2 - L3^2}{2 \cdot L2 \cdot R}\right)\right) \cdot \frac{180.00}{\pi} \\
\Theta_2 &= \alpha_1 + \alpha_2 \\
\Theta_3 &= \left(\arccos\left(\frac{L2^2 + L3^2 - R^2}{2 \cdot L2 \cdot L3}\right)\right) \cdot \frac{180.00}{\pi} \\
\Theta_4 &= 180.00 - \left((180.00 - (\alpha_2 + \Theta_3)) - \alpha_1\right)
\end{align*}
$$

当末端执行器位于第二象限时：
$$
\begin{align*}
\text{(公式与第一象限类似，但是要注意 $\Theta_2$ 和 $\Theta_4$ 的计算)} \\
\Theta_2 &= \alpha_2 - \alpha_1 \\
\Theta_4 &= 180.00 - \left((180.00 - (\alpha_2 + \Theta_3)) + \alpha_1\right)
\end{align*}
$$

当末端执行器位于第三象限时：
$$
\begin{align*}
\text{(公式与第一象限类似，但是要注意 $\Theta_1$ 的计算)} \\
\Theta_1 &= 90.00 \\
\Theta_2 &= \alpha_1 + \alpha_2 \\
\Theta_4 &= 180.00 - \left((180.00 - (\alpha_2 + \Theta_3)) - \alpha_1\right)
\end{align*}
$$

当末端执行器位于第四象限时：
\[
\begin{align*}
\text{(公式与第一象限类似，但是要注意 $\Theta_1$ 的计算)} \\
\Theta_1 &= 90.00 \\
\Theta_2 &= \alpha_2 - \alpha_1 \\
\Theta_4 &= 180.00 - \left((180.00 - (\alpha_2 + \Theta_3)) + \alpha_1\right)
\end{align*}
\]

当末端执行器位于 $X$ 轴上时：
\[
\begin{align*}
\text{(公式与第一象限类似，但是要注意 $\Theta_1$ 的计算)} \\
\Theta_1 &= 90.00 \\
\Theta_2 &= \alpha_1 + \alpha_2 \\
\Theta_4 &= 180.00 - \left((180.00 - (\alpha_2 + \Theta_3)) - \alpha_1\right)
\end{align*}
\]

当末端执行器位于 $Y$ 轴上时：
\[
\begin{align*}
\text{(公式与第一象限类似，但是要注意 $\Theta_1$ 的计算)} \\
\Theta_1 &= 90.00 + (90.00 - \left|\left(\arctan\left(\frac{Z_{\text{End\_Effector}}}{X_{\text{End\_Effector}}}\right)\right)\right| \cdot \frac{180.00}{\pi} \\
\Theta_2 &= \alpha_1 + \alpha_2 \\
\Theta_4 &= 180.00 - \left((180.00 - (\alpha_2 + \Theta_3)) - \alpha_1\right)
\end{align*}
\]

## Trapzoidal profile generator
https://github.com/WRidder/MotionProfileGenerator.git