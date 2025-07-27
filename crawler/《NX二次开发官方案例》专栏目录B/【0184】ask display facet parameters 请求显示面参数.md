### 【0184】ask display facet parameters 请求显示面参数

#### 代码

```cpp
    static void ask_display_facet_parameters(UF_FACET_parameters_p_t params)  
    {  
        int  
            units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        double  
            cf = 1,  
            view_center[3],  
            view_scale;  
        UF_DISP_system_params_t  
            sys_params;  
        UF_CALL(UF_DISP_ask_system_parameters(&sys_params));  
        UF_CALL(uc6430("", view_center, &view_scale));  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if (units == UF_PART_METRIC) cf = 25.4;  
        UF_FACET_INIT_PARAMETERS(params);  
        params->specify_surface_tolerance = TRUE;  
        params->surface_dist_tolerance = cf * sys_params.facet_chord_tol/view_scale;  
        params->surface_angular_tolerance = sys_params.facet_angle_tol * DEGRA;  
        params->specify_curve_tolerance = TRUE;  
        params->curve_dist_tolerance = cf * sys_params.facet_edge_tol/view_scale;  
        params->curve_angular_tolerance = sys_params.facet_angle_tol * DEGRA;  
    }

```

#### 代码解析

> 这段代码是用于设置NX中Facet参数的二次开发代码。其主要功能如下：
>
> 1. 获取当前显示的部件，并询问其单位制。
> 2. 获取当前的视图中心和视图缩放比例。
> 3. 获取NX系统参数中的Facet弦高公差、角度公差、边公差等参数。
> 4. 根据单位制，将公差转换为毫米单位。
> 5. 根据视图缩放比例，将公差转换为当前视图的公差。
> 6. 设置Facet参数，包括曲面公差、曲线公差、角度公差等。
> 7. 曲面公差包括距离公差和角度公差。
> 8. 曲线公差包括距离公差和角度公差。
> 9. 公差值都经过视图缩放比例和单位转换。
> 10. 该函数通过设置Facet参数，可以控制Facet的显示质量。
>
> 总体来说，该代码通过获取系统参数和当前视图信息，计算并设置Facet的显示公差，以控制Facet的显示质量，是NX二次开发中常用的功能之一。
>
