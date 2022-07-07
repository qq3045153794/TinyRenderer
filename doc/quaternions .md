#### 四元数概念

四元数类似于复数 由一个实部和三个虚部组成 由于数学部分太复杂了(我也不会) 就不赘述了
但我们用到四元数这个工具的主要原因是
+ 四元数能让物体绕任意轴旋转
+ 四元数之间插值简单

#### 性质 

四元数是由四个数$[w,x,y,z]$组成,事实上相当于储存了个角度个旋转轴
```cpp
vec3 rotation_axis; // 旋转轴
rotation_angle;     // 旋转角
w = cos(rotation_angle);
x = rotation_axis.x * sin(rotation_angle / 2);
y = rotation_axis.y * sin(rotation_angle / 2);
z = rotation_axis.z * sin(rotation_angle / 2);  
```