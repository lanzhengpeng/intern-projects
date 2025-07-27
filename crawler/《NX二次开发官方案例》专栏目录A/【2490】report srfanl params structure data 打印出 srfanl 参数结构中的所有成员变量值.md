### 【2490】report srfanl params structure data 打印出 srfanl 参数结构中的所有成员变量值

#### 代码

```cpp
    /* 里海 */  
    static void report_srfanl_params( UF_DISP_srfanl_data_t *params )  
    {  
        WRITE_F( params->spike_length );     
        WRITE6F( params->ref_plane );  
        WRITE3F( params->ref_vector );  
        WRITE_F( params->mid_value );  
        WRITE_F( params->scale_factor );  
        WRITE_F( params->anal_facet_edge_tol );  
        WRITE_F( params->anal_facet_chord_tol );  
        WRITE_F( params->anal_facet_angle_tol );  
        WRITE_F( params->anal_facet_width_tol );  
        WRITE_D( params->surf_anal_data );  
        WRITE_D( params->surf_anal_fineness );  
        WRITE_D( params->refl_type );  
        WRITE_D( params->num_refl_lines );  
        WRITE_D( params->refl_horiz_vert );  
        WRITE_D( params->thick_refl_lines );  
        WRITE_D( params->reflectivity );  
        WRITE_D( params->refl_move_type );  
        WRITE_D( params->refl_move_position );  
        WRITE_D( params->refl_smooth_lines );  
        WRITE_S( params->refl_user_image_filename );  
        WRITE_D( params->num_contours );  
        WRITE_D( params->surf_anal_display );  
        WRITE_D( params->legend_color_type_gaussian );  
        WRITE_D( params->legend_color_num_gaussian );  
        WRITE_D( params->surf_anal_display_mean );  
        WRITE_F( params->mid_value_mean );  
        WRITE_F( params->scale_factor_mean );  
        WRITE_D( params->legend_color_type_mean );  
        WRITE_D( params->legend_color_num_mean );  
        WRITE_D( params->surf_anal_display_max );  
        WRITE_F( params->mid_value_max );  
        WRITE_F( params->scale_factor_max );  
        WRITE_D( params->legend_color_type_max );  
        WRITE_D( params->legend_color_num_max );  
        WRITE_D( params->surf_anal_display_min );  
        WRITE_F( params->mid_value_min );  
        WRITE_F( params->scale_factor_min );  
        WRITE_D( params->legend_color_type_min );  
        WRITE_D( params->legend_color_num_min );  
        WRITE_D( params->surf_anal_display_normal );  
        WRITE_F( params->mid_value_normal );  
        WRITE_F( params->scale_factor_normal );  
        WRITE_D( params->legend_color_type_normal );  
        WRITE_D( params->legend_color_num_normal );  
        WRITE_D( params->surf_anal_display_sec );  
        WRITE_F( params->mid_value_sec );  
        WRITE_F( params->scale_factor_sec );  
        WRITE_D( params->legend_color_type_sec );  
        WRITE_D( params->legend_color_num_sec );  
        WRITE_D( params->surf_anal_display_u );  
        WRITE_F( params->mid_value_u );  
        WRITE_F( params->scale_factor_u );  
        WRITE_D( params->legend_color_type_u );  
        WRITE_D( params->legend_color_num_u );  
        WRITE_D( params->surf_anal_display_v );  
        WRITE_F( params->mid_value_v );  
        WRITE_F( params->scale_factor_v );  
        WRITE_D( params->legend_color_type_v );  
        WRITE_D( params->legend_color_num_v );  
        WRITE_D( params->surf_anal_display_slope );  
        WRITE_F( params->mid_value_slope );  
        WRITE_F( params->scale_factor_slope );  
        WRITE_D( params->legend_color_type_slope );  
        WRITE_D( params->legend_color_num_slope );  
        WRITE_D( params->surf_anal_display_distance );  
        WRITE_F( params->mid_value_distance );  
        WRITE_F( params->scale_factor_distance );  
        WRITE_D( params->legend_color_type_distance );  
        WRITE_D( params->legend_color_num_distance );  
        WRITE_D( params->surf_anal_display_refl );  
    }

```

#### 代码解析

> 这是NX Open C++中的一个函数，用于打印和报告NX中表面分析(Surface Analysis)的参数。
>
> 主要功能包括：
>
> 1. 打印表面分析的参考平面、参考向量、中值、缩放因子等基本参数。
> 2. 打印表面分析的各种公差参数，如边缘公差、弦公差、角度公差、宽度公差等。
> 3. 打印表面分析的数据和精细度参数。
> 4. 打印反射分析的各种参数，如反射线数量、水平/垂直、厚度、反射率、移动类型、移动位置、平滑线、用户图像文件名等。
> 5. 打印等高线数量和显示参数。
> 6. 打印各种显示模式(均值、最大值、最小值、法线、切线、U、V、斜率、距离、反射)的中间值、缩放因子、颜色类型、颜色数量等参数。
>
> 总体来说，这个函数用于输出和报告NX表面分析的所有参数，方便调试和了解当前的参数设置。
>
