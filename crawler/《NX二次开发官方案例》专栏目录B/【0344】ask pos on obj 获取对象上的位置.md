### 【0344】ask pos on obj 获取对象上的位置

#### 代码

```cpp
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs_mx, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs_mx, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void ask_pos_on_obj(tag_t obj, double loc[3])  
    {  
        tag_t  
            line;  
        double  
            cp[3],  
            dist;  
        UF_CURVE_line_t  
            lp;  
        map_abs2view(loc);  
        lp.start_point[0] = loc[0];  
        lp.start_point[1] = loc[1];  
        lp.start_point[2] = loc[2] + 1000;  
        lp.end_point[0] = loc[0];  
        lp.end_point[1] = loc[1];  
        lp.end_point[2] = loc[2] - 1000;  
        map_view2abs(lp.start_point);  
        map_view2abs(lp.end_point);  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包括以下几个部分：
>
> 1. 坐标系转换函数：map_abs2view和map_view2abs用于实现绝对坐标系到视图坐标系以及视图坐标系到绝对坐标系的转换。这些函数通过NX提供的API函数实现坐标转换矩阵的计算。
> 2. 询问对象上点的位置：ask_pos_on_obj函数用于在指定对象上询问一个点的位置。它首先将输入点从绝对坐标系转换到视图坐标系，然后创建一条垂直线，并计算该线与指定对象的最小距离，从而获取点的位置。
> 3. 坐标系转换的实现：坐标系转换函数使用NX提供的API函数计算坐标转换矩阵，然后将输入点通过矩阵变换到目标坐标系。
> 4. 询问点位置的实现：通过在视图坐标系中创建垂直线，并计算该线与对象的最小距离，从而确定输入点在对象上的位置。
> 5. NX API函数的使用：代码中使用了多个NX提供的API函数，如UF_CALL,FTN,UF_DISP_set_display,UF_CURVE_create_line等，用于实现坐标系转换、对象创建、对象操作等功能。
> 6. 整体流程：代码首先实现坐标系转换，然后在视图坐标系中创建垂直线，接着计算该线与对象的最小距离，最后获取输入点在对象上的位置。
>
> 总体来说，这段代码实现了坐标系转换和对象上点位置的查询，展示了NX二次开发中坐标系处理和几何计算的基本思路和流程。
>
