### 【2257】report offsets between selected coordinate systems 报告选定的坐标系之间的偏移量

#### 代码

```cpp
    /*HEAD REPORT_OFFSETS_BETWEEN_SELECTED_COORDINATE_SYSTEMS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_mtx.h>  
    #include <uf_vec.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void map_abs2csys(tag_t input_csys, double loc[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[16];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(loc, mx);  
        UF_VEC3_sub(loc, csys, loc);  
    }  
    static void map_vec_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            origin[3],  
            mx[16];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
    }  
    static void transform_abs2csys(double orig[3], double csys[9], double mx[16])  
    {  
        double  
            tx1[12],  
            tx2[12];  
    /*  Invert the rotation from the destination matrix 里海译:将目的地矩阵的旋转进行反转 */  
        tx2[0] = csys[0];  
        tx2[1] = csys[3];  
        tx2[2] = csys[6];  
        tx2[3] = 0;  
        tx2[4] = csys[1];  
        tx2[5] = csys[4];  
        tx2[6] = csys[7];  
        tx2[7] = 0;  
        tx2[8] = csys[2];  
        tx2[9] = csys[5];  
        tx2[10] = csys[8];  
        tx2[11] = 0;  
    /*  set up to translate from abs to the destination csys 里海译:根据文档内容，设置从ABS到目标CSYS的转换。 */  
        FTN(uf5943)(orig, tx1);  
    /*  combine this with the inverted rotation csys above 里海译:与上面反转的旋转坐标系相结合 */  
        FTN(uf5942)(tx2, tx1, mx);  
    }  
    static void report_csys_to_csys_offsets(tag_t ref_csys, tag_t dest_csys)  
    {  
        double  
            angle_x,  
            angle_y,  
            angle_z,  
            delta[3],  
            from_orig[3],  
            mx[16],  
            r[9],  
            to_csys[9],  
            to_orig[3],  
            zero[3] = { 0,0,0 };  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, from_orig));  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, to_orig));  
        map_abs2csys(ref_csys, from_orig);  
        map_abs2csys(ref_csys, to_orig);  
        UF_VEC3_sub(to_orig, from_orig, delta);  
        ECHO("Translation:\n");  
        ECHO("  X = %f\n", delta[0]);  
        ECHO("  Y = %f\n", delta[1]);  
        ECHO("  Z = %f\n", delta[2]);  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, to_csys));  
        map_vec_abs2csys(ref_csys, to_csys);  
        map_vec_abs2csys(ref_csys, &to_csys[3]);  
        map_vec_abs2csys(ref_csys, &to_csys[6]);  
        transform_abs2csys(zero, to_csys, mx);  
        UF_MTX4_ask_rotation(mx, r);  
    //  See http://www.gregslabaugh.name/publications/euler.pdf  
        ECHO("Rotation:\n");  
        if (fabs(r[6]) != 1)  
        {  
            ECHO("  solution 1 of 2:\n");  
            angle_y = -asin(r[6]);  
            angle_x = atan2((r[7]/cos(angle_y)), (r[8]/cos(angle_y)));  
            angle_z = atan2((r[3]/cos(angle_y)), (r[0]/cos(angle_y)));  
            ECHO("    Angle X = %f\n", angle_x * RADEG);  
            ECHO("    Angle Y = %f\n", angle_y * RADEG);  
            ECHO("    Angle Z = %f\n", angle_z * RADEG);  
            ECHO("  solution 2 of 2:\n");  
            angle_y = PI - angle_y;  
            angle_x = atan2((r[7]/cos(angle_y)), (r[8]/cos(angle_y)));  
            angle_z = atan2((r[3]/cos(angle_y)), (r[0]/cos(angle_y)));  
            ECHO("    Angle X = %f\n", angle_x * RADEG);  
            ECHO("    Angle Y = %f\n", angle_y * RADEG);  
            ECHO("    Angle Z = %f\n", angle_z * RADEG);  
        }  
        else  
        {  
            angle_z = 0; // anything, e.g. 0  
            if (r[6] == -1)  
            {  
                angle_y = PI/2;  
                angle_x = angle_z + atan2(r[1], r[2]);  
            }  
            else  
            {  
                angle_y = -PI/2;  
                angle_x = -angle_z + atan2(-r[1], -r[2]);  
            }  
            ECHO("  Angle X = %f\n", angle_x * RADEG);  
            ECHO("  Angle Y = %f\n", angle_y * RADEG);  
            ECHO("  Angle Z = %f\n", angle_z * RADEG);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            d_csys,  
            r_csys;  
        while ((r_csys = select_a_csys("Reference csys")) &&  
               (d_csys = select_a_csys("Destination csys")))  
        {  
            report_csys_to_csys_offsets(r_csys, d_csys);  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 提供了一个对话框，用于选择参考坐标系和目标坐标系。
> 2. 计算了参考坐标系到目标坐标系的平移距离。
> 3. 计算了参考坐标系到目标坐标系的旋转角度。
> 4. 输出结果包括平移距离和旋转角度。
> 5. 在用户选择参考坐标系和目标坐标系时，程序会高亮显示选中的坐标系。
> 6. 使用了UF_CALL宏来检查NX API函数的返回码，并在出错时输出错误信息。
> 7. 定义了多个辅助函数，包括选择坐标系、坐标系信息查询、坐标转换等。
> 8. ufusr是程序的入口函数，首先初始化NX环境，然后调用do_it函数执行主体逻辑，最后终止NX环境。
> 9. ufusr_ask_unload函数用于卸载程序。
>
> 总体来说，这段代码实现了NX中坐标系之间变换的查询功能，通过对话框交互选择坐标系，并计算出平移和旋转参数，最后输出结果。
>
