### 【2044】report brep geometry 报告BREP几何信息

#### 代码

```cpp
    static void report_brep_geometry(UF_BREP_geometry_t *geometry)  
    {  
        int  
            ii;  
        UF_UI_open_listing_window();  
        WRITE("geometry->type = ");  
        switch (geometry->type)  
        {  
            case UF_BREP_POINT_GEOM:  
                WRITE_D(UF_BREP_POINT_GEOM);  
                WRITE3F(geometry->geom.point);  
                break;  
            case UF_BREP_LINE_GEOM:  
                WRITE_D(UF_BREP_LINE_GEOM);  
                WRITE3F(geometry->geom.line.start_point);  
                WRITE3F(geometry->geom.line.end_point);  
                break;  
            case UF_BREP_SPLINE_GEOM:  
                WRITE_D(UF_BREP_SPLINE_GEOM);  
                WRITE_D(geometry->geom.spline.num_poles);  
                WRITE_D(geometry->geom.spline.order);  
                WRITE_L(geometry->geom.spline.is_rational);  
                WRITEnF(geometry->geom.spline.knots,  
                    geometry->geom.spline.num_poles + geometry->geom.spline.order);  
                WRITEnF4((double *)geometry->geom.spline.poles,  
                    geometry->geom.spline.num_poles);  
                WRITE_F(geometry->geom.spline.start_param);  
                WRITE_F(geometry->geom.spline.end_param);  
                break;  
            case UF_BREP_ARC_GEOM:  
                WRITE_D(UF_BREP_ARC_GEOM);  
                WRITE_D(geometry->geom.arc.matrix_tag);  
                WRITE_F(geometry->geom.arc.start_angle * RADEG);  
                WRITE_F(geometry->geom.arc.end_angle * RADEG);  
                WRITE3F(geometry->geom.arc.arc_center);  
                WRITE_F(geometry->geom.arc.radius);  
                break;  
            case UF_BREP_ELLIPSE_GEOM:  
            case UF_BREP_HYPERBOLA_GEOM:  
            case UF_BREP_PARABOLA_GEOM:  
                WRITE_D(UF_BREP_ELLIPSE_GEOM);  
                WRITE_D(geometry->geom.conic.matrix_tag);  
                WRITE_D(geometry->geom.conic.conic_type);  
                WRITE_F(geometry->geom.conic.rotation_angle);  
                WRITE_F(geometry->geom.conic.start_param);  
                WRITE_F(geometry->geom.conic.end_param);  
                WRITE3F(geometry->geom.conic.center);  
                WRITE_F(geometry->geom.conic.k1);  
                WRITE_F(geometry->geom.conic.k2);  
                break;  
            case UF_BREP_INTERSECTION_GEOM:  
                WRITE_D(UF_BREP_INTERSECTION_GEOM);  
                break;  
            case UF_BREP_COMPOSITE_GEOM:  
                WRITE_D(UF_BREP_COMPOSITE_GEOM);  
                WRITE_D(geometry->geom.composite.num_curves);  
                for (ii = 0; ii < geometry->geom.composite.num_curves; ii++)  
                {  
                    WRITE_D(ii);  
                    report_brep_geometry(&(geometry->geom.composite.curves[ii]));  
                }  
                break;  
            case UF_BREP_CYLINDER_GEOM:  
                WRITE_D(UF_BREP_CYLINDER_GEOM);  
                WRITE3F(geometry->geom.cylinder.pt_on_axis);  
                WRITE3F(geometry->geom.cylinder.axis);  
                WRITE3F(geometry->geom.cylinder.u0_ref_direc);  
                WRITE_F(geometry->geom.cylinder.radius);  
                break;  
            case UF_BREP_CONE_GEOM:  
                WRITE_D(UF_BREP_CONE_GEOM);  
                WRITE3F(geometry->geom.cone.pt_at_apex);  
                WRITE3F(geometry->geom.cone.axis);  
                WRITE3F(geometry->geom.cone.u0_ref_direc);  
                WRITE_F(geometry->geom.cone.half_apex_angle);  
                break;  
            case UF_BREP_SPHERE_GEOM:  
                WRITE_D(UF_BREP_SPHERE_GEOM);  
                WRITE3F(geometry->geom.sphere.center);  
                WRITE3F(geometry->geom.sphere.axis);  
                WRITE3F(geometry->geom.sphere.u0_ref_direc);  
                WRITE_F(geometry->geom.sphere.radius);  
                break;  
            case UF_BREP_TORUS_GEOM:  
                WRITE_D(UF_BREP_TORUS_GEOM);  
                WRITE3F(geometry->geom.torus.center);  
                WRITE3F(geometry->geom.torus.axis);  
                WRITE3F(geometry->geom.torus.u0_ref_direc);  
                WRITE_F(geometry->geom.torus.major_radius);  
                WRITE_F(geometry->geom.torus.minor_radius);  
                break;  
            case UF_BREP_REVOLVE_GEOM:  
                WRITE_D(UF_BREP_REVOLVE_GEOM);  
                WRITE3F(geometry->geom.revolve.pt_on_axis);  
                WRITE3F(geometry->geom.revolve.rot_axis);  
                report_brep_geometry(geometry->geom.revolve.generator);  
                break;  
            case UF_BREP_EXTRUDE_GEOM:  
                WRITE_D(UF_BREP_EXTRUDE_GEOM);  
                WRITE3F(geometry->geom.extrude.direction);  
                report_brep_geometry(geometry->geom.extrude.generator);  
                break;  
            case UF_BREP_BSURFACE_GEOM:  
                WRITE_D(UF_BREP_BSURFACE_GEOM);  
                WRITE_D(geometry->geom.bsurface.num_poles_u);  
                WRITE_D(geometry->geom.bsurface.num_poles_v);  
                WRITE_D(geometry->geom.bsurface.order_u);  
                WRITE_D(geometry->geom.bsurface.order_v);  
                WRITE_L(geometry->geom.bsurface.is_rational);  
                WRITEnF(geometry->geom.bsurface.knots_u,  
                    geometry->geom.bsurface.num_poles_u +  
                    geometry->geom.bsurface.order_u);  
                WRITEnF(geometry->geom.bsurface.knots_v,  
                    geometry->geom.bsurface.num_poles_v +  
                    geometry->geom.bsurface.order_v);  
                WRITEnF4((double *)geometry->geom.bsurface.poles,  
                    geometry->geom.bsurface.num_poles_u *  
                    geometry->geom.bsurface.num_poles_v );  
                break;  
            case UF_BREP_OFFSET_GEOM:  
                WRITE_D(UF_BREP_OFFSET_GEOM);  
                report_brep_geometry(geometry->geom.offset.base);  
                WRITE_F(geometry->geom.offset.distance);  
                break;  
            case UF_BREP_PLANE_GEOM:  
                WRITE_D(UF_BREP_PLANE_GEOM);  
                WRITE3F(geometry->geom.plane.pt_on_plane);  
                WRITE3F(geometry->geom.plane.normal);  
                WRITE3F(geometry->geom.plane.u0_ref_direc);  
                break;  
            case UF_BREP_BLEND_GEOM:  
                WRITE_D(UF_BREP_BLEND_GEOM);  
                break;  
            case UF_BREP_FORSURF_GEOM:  
                WRITE_D(UF_BREP_FORSURF_GEOM);  
                WRITE_S(geometry->geom.foreign.char_data);  
                WRITE_D(geometry->geom.foreign.nspace);  
                WRITE_D(geometry->geom.foreign.num_ints);  
                WRITEnD(geometry->geom.foreign.int_array,  
                    geometry->geom.foreign.num_ints);  
                WRITE_D(geometry->geom.foreign.num_doubles);  
                WRITEnF(geometry->geom.foreign.double_array,  
                    geometry->geom.foreign.num_doubles);  
                WRITE16F((double *)geometry->geom.foreign.transform);  
                break;  
            case UF_BREP_UVBOX_GEOM:  
                WRITE_D(UF_BREP_UVBOX_GEOM);  
                WRITE_F(geometry->geom.uvbox.umin);  
                WRITE_F(geometry->geom.uvbox.umax);  
                WRITE_F(geometry->geom.uvbox.vmin);  
                WRITE_F(geometry->geom.uvbox.vmax);  
                break;  
            case UF_BREP_TAG_GEOM:  
                WRITE_D(UF_BREP_TAG_GEOM);  
                WRITE_D(geometry->geom.tagged.object);  
                break;  
            default:  
                WRITE("Unknown geometry type - ");  
                WRITE_D(geometry->type);  
                break;  
        }  
    }

```

#### 代码解析

> 这段代码是用于报告NX中BREP（边界表示）几何信息的函数。
>
> 函数主要功能包括：
>
> 1. 打开一个列表窗口用于输出信息。
> 2. 根据几何的类型，输出不同几何的信息。几何类型包括点、线、样条、圆弧、椭圆、超椭圆、抛物线、交线、复合线、圆柱、圆锥、球、环面、旋转、拉伸、双曲面、偏移、平面、混合面、外部面、UV框、标签等。
> 3. 对于每种几何，输出其关键信息，如点坐标、线端点、样条参数、圆弧中心、椭圆中心、圆柱中心、球中心等。
> 4. 对于复合几何，递归输出其包含的所有曲线的几何信息。
> 5. 对于双曲面，输出其U向和V向的极点和节点信息。
> 6. 对于偏移几何，输出其基线和偏移距离。
> 7. 对于外部几何，输出其字符数据、命名空间、整数数组、浮点数组、变换矩阵等信息。
> 8. 对于UV框，输出其U向和V向的最小最大值。
> 9. 对于标签几何，输出其关联的对象。
> 10. 对于未知的几何类型，输出提示信息。
>
> 该函数通过递归调用，可以输出BREP几何的详细信息，用于调试和诊断NX中的几何数据。
>
