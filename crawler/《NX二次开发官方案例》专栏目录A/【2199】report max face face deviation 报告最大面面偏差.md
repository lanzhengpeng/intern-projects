### 【2199】report max face face deviation 报告最大面面偏差

#### 代码

```cpp
    /*HEAD REPORT_MAX_FACE_FACE_DEVIATION CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，UF_print_syslog 是 V18 版本新增的功能。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void build_temp_filespec(char *fspec, int ftype)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(tmp_dir, ftype, part_name, fspec));  
    }  
    /*  This function demonstrates using the GRIP command DEVCHK to do  
        face to face deviation checking from an API program.  
        The code for the GRIP program called by this program is:  
            ENTITY/ one_face, two_face  
            NUMBER/ u_pts, v_pts, d_tol, a_tol  
            UFARGS/ one_face, two_face, u_pts, v_pts, d_tol, a_tol  
            DRAW/OFF  
            DEVCHK/ one_face, TO, two_face, u_pts, v_pts, TOLER, d_tol, a_tol  
            DRAW/ON  
            HALT  
    */  
    static double ask_max_face_face_deviation(  
        tag_t face1,  
        tag_t face2,  
        int u_check_points,  
        int v_check_points,  
        double distance_tolerance,  
        double angle_tolerance)  
    {  
        double  
            max_dev = -1,  
            U_check = u_check_points,  
            V_check = v_check_points;  
        char  
            a_line[80],  
            tfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_args_t  
            args[6];  
        FILE  
            *data_file;  
        args[0].type = UF_TYPE_TAG_T;  
        args[0].length = 0;  
        args[0].address = &face1;  
        args[1].type = UF_TYPE_TAG_T;  
        args[1].length = 0;  
        args[1].address = &face2;  
        args[2].type = UF_TYPE_DOUBLE;  
        args[2].length = 0;  
        args[2].address = &U_check;  
        args[3].type = UF_TYPE_DOUBLE;  
        args[3].length = 0;  
        args[3].address = &V_check;  
        args[4].type = UF_TYPE_DOUBLE;  
        args[4].length = 0;  
        args[4].address = &distance_tolerance;  
        args[5].type = UF_TYPE_DOUBLE;  
        args[5].length = 0;  
        args[5].address = &angle_tolerance;  
        build_temp_filespec(tfn, 4);  
        UF_CALL(uc4400(4, tfn, 0));  
        UF_CALL(UF_call_grip("Z:\\grip\\devchk.grx", 6, args));  
        UF_CALL(uc4400(0, tfn, 0));  
        if ((data_file = fopen(tfn, "r")) != NULL)  
        {  
            while (!feof(data_file))  
            {  
                fgets(a_line, 80, data_file);  
                if (strstr(a_line, "Maximum Distance Error"))  
                {  
                    sscanf(a_line, "Maximum Distance Error   = %lf", &max_dev);  
                    break;  
                }  
            }  
            fclose(data_file);  
        }  
        UF_CALL(uc4561(tfn, 4));  
        return max_dev;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face1,  
            face2;  
        while (((face1 = select_a_face("Select first face")) != NULL_TAG) &&  
               ((face2 = select_a_face("Select second face")) != NULL_TAG))  
            WRITE_F(ask_max_face_face_deviation(face1, face2, 20, 20, 0.001, 0.5));  
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

> 这是一段用于在NX中测量两个面之间最大偏差的二次开发代码。主要步骤包括：
>
> 1. 定义了一个报告错误函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个用于选择面的函数select_a_face，该函数使用对话框让用户选择一个面，并返回面的标识。
> 3. 定义了一个向列表窗口输出双精度数的函数write_double_to_listing_window。
> 4. 定义了一个构建临时文件路径的函数build_temp_filespec。
> 5. 定义了一个主函数do_it，该函数循环让用户选择两个面，然后调用ask_max_face_face_deviation函数计算这两个面之间的最大偏差，并输出到列表窗口。
> 6. ask_max_face_face_deviation函数首先构建GRIP脚本参数，调用GRIP脚本执行偏差检查，然后从GRIP脚本的输出文件中读取最大偏差值。
> 7. ufusr函数是二次开发程序的入口，初始化后调用do_it函数，最后终止程序。
> 8. ufusr_ask_unload函数用于卸载二次开发程序，返回立即卸载的标志。
>
> 总的来说，这段代码实现了在NX中测量两个面之间最大偏差的功能，利用了GRIP脚本进行计算。
>
