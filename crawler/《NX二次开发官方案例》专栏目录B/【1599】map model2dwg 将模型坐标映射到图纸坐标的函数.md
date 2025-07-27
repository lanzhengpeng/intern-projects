### 【1599】map model2dwg 将模型坐标映射到图纸坐标的函数

#### 代码

```cpp
    /*  In V16 and higher use: UF_VIEW_map_model_to_drawing 译:In V16 and higher use: UF_VIEW_map_model_to_drawing */  
    static void map_model2dwg(char *dwg, char *member_view, double pos[3])  
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
            dsp_prt_tag;  
        int  
            dcode,  
            dunits,  
            punits;  
        /* Get the view reference point and scale 译:获取视图参考点和缩放比例。 */  
        UF_CALL(uc6442(member_view, mdl_ref, &scale));  
        /* Get the drawing reference point 译:获取绘图参考点 */  
        UF_CALL(uc6483(dwg, member_view, dwg_ref));  
        /* Get view rotation matrix 译:获取视图旋转矩阵 */  
        UF_CALL(uc6433(member_view, mx));  
        /* Get drawing units (1 = inches, 2 = mm) 译:获取绘图单位（1 = 英寸，2 = 毫米） */  
        UF_CALL(uc6479(dwg, &dunits, &dcode, dsize));  
        if (dunits == 2) factor = 25.4;  
        /* Get part units (1 = mm, 2 = inches) 译:获取零件单位（1=毫米，2=英寸） */  
        dsp_prt_tag = UF_PART_ask_display_part( );  
        UF_CALL(UF_PART_ask_units( dsp_prt_tag, &punits ));  
        if (punits == 1) factor = factor / 25.4;  
        /* Subtract the view reference point 译:翻译：减去视图参考点 */  
        UF_VEC3_sub(pos, mdl_ref, pos);  
        /* Map through the view rotation matrix 译:通过视图旋转矩阵映射 */  
        UF_MTX3_vec_multiply(pos, mx, tmp_pos);  
        /* Scale by the view's drawing scale 译:按视图的绘制比例缩放 */  
        UF_VEC3_scale(scale, tmp_pos, pos);  
        /* Convert to drawing units 译:将单位转换为绘图单位 */  
        UF_VEC3_scale(factor, pos, pos);  
        /* Add the drawing reference point 译:增加绘图参考点 */  
        UF_VEC3_add(pos, dwg_ref, pos);  
        /* Convert back to part units 译:将背部转换为部件单位。 */  
        UF_VEC3_scale(1/factor, pos, pos);  
        /* Set Z to 0.0 译:将Z设置为0.0。 */  
        pos[2] = 0.0;  
    }

```

#### 代码解析

> 这段代码主要用于将NX模型中的点映射到对应的DWG图纸上的点。具体步骤包括：
>
> 1. 获取视图的参考点和缩放比例。
> 2. 获取DWG图纸的参考点。
> 3. 获取视图的旋转矩阵。
> 4. 获取图纸和模型使用的单位，并进行单位转换。
> 5. 从模型点中减去视图的参考点。
> 6. 通过视图的旋转矩阵将模型点转换到图纸坐标系。
> 7. 根据视图的缩放比例调整图纸上的点。
> 8. 转换图纸上的点到图纸单位。
> 9. 加上图纸的参考点。
> 10. 转换回模型单位。
> 11. 将Z坐标设置为0。
> 12. 最终获得模型点在DWG图纸上的对应点。
>
> 这段代码主要用于在NX模型和DWG图纸之间进行坐标转换，以便在图纸和模型之间进行映射和关联。
>
