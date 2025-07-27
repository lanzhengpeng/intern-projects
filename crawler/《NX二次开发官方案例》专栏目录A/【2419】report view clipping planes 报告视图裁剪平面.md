### 【2419】report view clipping planes 报告视图裁剪平面

#### 代码

```cpp
    static void report_view_clipping_planes(char *view)  
    {  
        int  
            status[2];  
        double  
            distances[2];  
        UF_CALL(UF_UI_open_listing_window());  
        WRITE_D(uc6436(view, status, distances));  
        WRITE_D(status[0]);  
        WRITE_D(status[1]);  
        if (status[0] > 1) WRITE_F(distances[0]);  
        if (status[1] > 1) WRITE_F(distances[1]);  
    }

```

#### 代码解析

> 这段代码是用于报告NX视图中裁剪平面状态的函数。
>
> 具体功能包括：
>
> 1. 打开NX的输出窗口。
> 2. 调用UF_UI_open_listing_window函数，获取当前视图的裁剪平面状态(status数组)和距离(distances数组)。
> 3. 输出裁剪平面状态，即status数组中的两个元素，如果裁剪平面打开，则输出距离值。
> 4. 使用WRITE_D宏输出整型值，使用WRITE_F宏输出浮点型值。
>
> 这个函数的作用是让开发者可以查看当前视图的裁剪平面状态，包括是否打开以及距离等信息，以便进行调试或了解当前视图配置。
>
