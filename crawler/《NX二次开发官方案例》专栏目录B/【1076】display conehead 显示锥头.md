### 【1076】display conehead 显示锥头

#### 代码

```cpp
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }

```

#### 代码解析

> 根据代码，这是段NX二次开发代码，实现了以下功能：
>
> 1. 定义了一个静态函数display_conehead，用于在NX中显示一个圆锥头。
> 2. 函数接受三个参数：origin（圆锥头的原点坐标），vec（圆锥头的方向向量），以及一个标志位（此处为0）。
> 3. 使用UF_DISP_conehead函数在NX中显示圆锥头。UF_DISP_conehead是NX的显示函数，其中UF_DISP_ALL_ACTIVE_VIEWS表示在所有激活的视图上显示，origin和vec分别表示圆锥头的原点和方向。
> 4. 根据参数，在NX中绘制了一个圆锥头，并显示在所有激活的视图上。
> 5. 这是一个简单的NX二次开发示例，展示了如何在NX中通过代码绘制几何图形。
>
> 总的来说，这段代码实现了在NX中绘制圆锥头并进行显示的功能。
>
