### 【2819】set wcs by conic object 根据圆锥曲线对象设置WCS

#### 代码

```cpp
    static tag_t set_wcs_by_x_axis_yaxis(double *x_axis, double *y_axis,  
        double *origin)  
    {  
        double  
            csys[9];  
        tag_t  
            mx,  
            orig_wcs,  
            wcs;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_MTX3_initialize(x_axis, y_axis, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }  
    static tag_t set_wcs_by_conic_object(tag_t object)  
    {  
        logical  
            is_it;  
        tag_t  
            old_wcs;  
        UF_EVAL_p_t  
            eval;  
        UF_EVAL_arc_t  
            arc;  
        UF_EVAL_ellipse_t  
            ellipse;  
        UF_EVAL_parabola_t  
            parabola;  
        UF_EVAL_hyperbola_t  
            hyperbola;  
        UF_CALL(UF_EVAL_initialize(object, &eval));  
        UF_CALL(UF_EVAL_is_arc(eval, &is_it));  
        if (is_it)  
        {  
            UF_EVAL_ask_arc(eval, &arc);  
            old_wcs = set_wcs_by_x_axis_yaxis(arc.x_axis, arc.y_axis, arc.center);  
        }  
        UF_CALL(UF_EVAL_is_ellipse(eval, &is_it));  
        if (is_it)  
        {  
            UF_EVAL_ask_ellipse(eval, &ellipse);  
            old_wcs = set_wcs_by_x_axis_yaxis(ellipse.x_axis, ellipse.y_axis,  
                ellipse.center);  
        }  
        UF_CALL(UF_EVAL_is_parabola(eval, &is_it));  
        if (is_it)  
        {  
            UF_EVAL_ask_parabola(eval, &parabola);  
            old_wcs = set_wcs_by_x_axis_yaxis(parabola.x_axis, parabola.y_axis,  
                parabola.center);  
        }  
        UF_CALL(UF_EVAL_is_hyperbola(eval, &is_it));  
        if (is_it)  
        {  
            UF_EVAL_ask_hyperbola(eval, &hyperbola);  
            old_wcs = set_wcs_by_x_axis_yaxis(hyperbola.x_axis, hyperbola.y_axis,  
                hyperbola.center);  
        }  
        UF_CALL(UF_EVAL_free(eval));  
        return old_wcs;  
    }

```

#### 代码解析

> 这段代码主要用于NX的二次开发，主要实现了两个功能：
>
> 1. 根据X轴和Y轴设置工作坐标系(WCS)：set_wcs_by_x_axis_yaxis函数接受X轴、Y轴和原点坐标，通过这些参数创建一个临时的坐标系，并设置它为当前的工作坐标系。
> 2. 根据二次曲线对象设置工作坐标系：set_wcs_by_conic_object函数首先检查输入对象是否为二次曲线，然后根据其类型(如圆弧、椭圆、抛物线、双曲线等)获取相应的参数，最后调用set_wcs_by_x_axis_yaxis函数来设置工作坐标系。
>
> 这些函数的目的是为后续操作提供方便的坐标系，以便进行后续的几何建模或分析工作。通过设置与模型几何特征相关的坐标系，可以简化后续的几何运算过程。
>
