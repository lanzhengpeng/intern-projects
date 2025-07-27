### 【0169】ask current view corners 获取当前视图的角点坐标

#### 代码

```cpp
    static void ask_current_view_corners(double corners[4][3])  
    {  
        double  
            bounds[4];  
        uc6450("", "", 0, 0);  
        UF_CALL(UF_VIEW_ask_xy_clip(NULL_TAG, bounds));  
        corners[0][0] = bounds[0];  
        corners[0][1] = bounds[2];  
        corners[0][2] = 0;  
        map_current_view2abs(&corners[0][0]);  
        corners[1][0] = bounds[1];  
        corners[1][1] = bounds[2];  
        corners[1][2] = 0;  
        map_current_view2abs(&corners[1][0]);  
        corners[2][0] = bounds[1];  
        corners[2][1] = bounds[3];  
        corners[2][2] = 0;  
        map_current_view2abs(&corners[2][0]);  
        corners[3][0] = bounds[0];  
        corners[3][1] = bounds[3];  
        corners[3][2] = 0;  
        map_current_view2abs(&corners[3][0]);  
    }

```

#### 代码解析

> 这段代码是一个用于获取当前NX视图四个角坐标的函数。其主要步骤包括：
>
> 1. 获取当前视图的x和y方向裁剪边界。
> 2. 将视图坐标系下的四个角点坐标转换到绝对坐标系下。
> 3. 返回转换后的四个角点坐标。
>
> 具体来说，函数首先调用UF_VIEW_ask_xy_clip获取视图的x和y方向裁剪边界，然后根据边界计算出视图坐标系下的四个角点坐标。接着，调用map_current_view2abs函数将视图坐标系下的坐标转换到绝对坐标系下。最后，返回转换后的四个角点坐标。
>
> 该函数的作用是获取当前视图在绝对坐标系下的范围，可用于后续的视图操作。
>
