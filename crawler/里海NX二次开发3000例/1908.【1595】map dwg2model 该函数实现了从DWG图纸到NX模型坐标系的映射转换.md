### 【1595】map dwg2model 该函数实现了从DWG图纸到NX模型坐标系的映射转换

#### 代码

```cpp
    static void map_dwg2model(double pos[3], tag_t member_view)  
    {  
        double  
            dwg_ref[3],  
            dsize[2],  
            factor = 1,  
            mdl_ref[3],  
            mx[9],  
            scale,  
            tmp_pos[3];  
        tag_t  
            dsp_prt_tag,  
            pnt;  
        int  
            dcode,  
            dunits,  
            n,  
            punits;  
        char  
            cur_dwg[MAX_ENTITY_NAME_SIZE] = { "" },  
            member_view_name[MAX_ENTITY_NAME_SIZE];  
        /* Get the view reference point and scale 译:获取视图参考点和缩放 */  
        UF_CALL(uc5027(member_view, member_view_name, &n));  
        UF_CALL(uc6442(member_view_name, mdl_ref, &scale));  
        /* Get the drawing reference point 译:获取绘图参考点 */  
        UF_CALL(uc6483(cur_dwg, member_view_name, dwg_ref));  
        /* Get view rotation matrix 译:获取视图旋转矩阵 */  
        UF_CALL(uc6433(member_view_name, mx));  
        /* Get drawing units (1 = inches, 2 = mm) 译:获取绘图单位(1 = 英寸，2 = 毫米) */  
        UF_CALL(uc6479(cur_dwg, &dunits, &dcode, dsize));  
        if (dunits == 2) factor = 25.4;  
        /* Get part units (1 = mm, 2 = inches) 译:获取零件单位（1 = 毫米，2 = 英寸） */  
        dsp_prt_tag = UF_PART_ask_display_part( );  
        UF_CALL(UF_PART_ask_units( dsp_prt_tag, &punits ));  
        if (punits == 1) factor = factor / 25.4;  
        /* Create a point on the drawing 译:在图纸中创建一个点。 */  
        UF_CALL(UF_CURVE_create_point(pos, &pnt));  
        /* Convert to drawing units 译:转换到绘图单位 */  
        UF_VEC3_scale(factor, pos, pos);  
        /* Subtract the drawing reference point 译:Subtract the drawing reference point */  
        UF_VEC3_sub(pos, dwg_ref, pos);  
        /* Convert back to part units 译:把部分单位转换回去 */  
        UF_VEC3_scale(1/factor, pos, pos);  
        /* Map through the transform of the view rotation matrix 译:通过视图旋转矩阵转换映射 */  
        UF_MTX3_vec_multiply_t(pos, mx, tmp_pos);  
        /* Scale down by the view's drawing scale 译:缩小视图的绘图比例尺 */  
        UF_VEC3_scale(1/scale, tmp_pos, pos);  
        /* Add the view reference point 译:添加视图参考点 */  
        UF_VEC3_add(pos, mdl_ref, pos);  
        /* Create a point in the member view 译:创建一个会员视图中的点 */  
        UF_CALL(UF_CURVE_create_point(pos, &pnt));  
        UF_CALL(uc6401("", pnt));  
        UF_CALL(uc6402(member_view_name, pnt));  
    }

```

#### 代码解析

> 这段代码是一个用于NX二次开发的函数，其主要功能是将一个点从绘图坐标系转换到模型坐标系。具体步骤如下：
>
> 1. 获取视图信息：通过函数调用，获取当前视图的名称、参考点、缩放比例和旋转矩阵。
> 2. 获取绘图单位：调用函数获取当前绘图单位，用于后续坐标转换。
> 3. 创建点：在绘图坐标系中创建一个点。
> 4. 坐标转换：首先将点坐标从模型单位转换为绘图单位，然后减去绘图参考点，得到相对坐标。
> 5. 应用视图变换：通过视图的旋转矩阵对点坐标进行变换。
> 6. 缩放处理：根据视图的缩放比例对点坐标进行缩放。
> 7. 转换回模型单位：将点坐标转换回模型单位。
> 8. 添加视图参考点：将点坐标加上视图参考点，得到模型坐标系中的点坐标。
> 9. 创建模型点：在模型坐标系中创建一个点。
> 10. 删除临时点：删除绘图坐标系中的临时点。
>
> 通过以上步骤，实现了点从绘图坐标系到模型坐标系的转换。该函数可以应用于NX二次开发中，用于实现绘图坐标系与模型坐标系之间的坐标转换。
>
