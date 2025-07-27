### 【2470】rotate copy selected csys about one of its own axes 旋转复制选定的坐标系，围绕其自身的一个轴

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
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
    static logical pick_one_of_three_choices(char *prompt, char *option_one,  
        char *option_two, char *option_three, int *choice)  
    {  
        int  
            resp;  
        char  
            options[3][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        strncpy(&options[2][0], option_three, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        options[2][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 3);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 , 2 or 3 里海译:翻译为：返回1、2或3。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void ask_csys_info(tag_t csys, double origin[3], double axes[9])  
    {  
        tag_t  
            matrix;  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, axes));  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            which_axis = 1;  
        tag_t  
            csys = NULL_TAG,  
            mx,  
            new_csys;  
        double  
            delta_degrees = 90,  
            delta_radians,  
            old_axes[9],  
            origin[3],  
            new_axes[9],  
            rotation_mx[9],  
            vector[3];  
        while (((csys = select_a_csys("Rotate/copy")) != NULL_TAG) &&  
            pick_one_of_three_choices("Rotate about", "Its X-axis", "Its Y-axis",  
                "Its Z-axis", &which_axis) &&  
            prompt_for_a_number("Rotate how far?", "Degrees", &delta_degrees))  
        {  
            set_undo_mark("Rotate/copy csys");  
            ask_csys_info(csys, origin, old_axes);  
            UF_VEC3_copy(&(old_axes[(which_axis-1)*3]), vector);  
            delta_radians = delta_degrees*DEGRA;  
            UF_MTX3_rotate_about_axis(vector, delta_radians, rotation_mx);  
            UF_MTX3_multiply_t(rotation_mx, old_axes, new_axes);  
            UF_CALL(UF_CSYS_create_matrix(new_axes, &mx));  
            UF_CALL(UF_CSYS_create_csys(origin, mx, &new_csys));  
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

> 这段NX二次开发代码的主要功能是选择一个坐标系，然后围绕其X轴、Y轴或Z轴旋转一定角度，并复制得到一个新的坐标系。
>
> 具体步骤如下：
>
> 1. 定义了错误报告函数report_error，用于打印出错误信息。
> 2. 定义了函数mask_for_csys，用于设置选择过滤条件，只允许选择坐标系。
> 3. 定义了函数select_a_csys，用于弹出选择对话框，让用户选择一个坐标系。
> 4. 定义了函数pick_one_of_three_choices，用于弹出一个选择对话框，让用户选择绕X轴、Y轴或Z轴旋转。
> 5. 定义了函数prompt_for_a_number，用于弹出一个输入对话框，让用户输入旋转角度。
> 6. 定义了函数ask_csys_info，用于查询坐标系的原始矩阵和原点信息。
> 7. 定义了函数set_undo_mark，用于设置撤销标记。
> 8. 定义了函数do_it，用于主流程，循环选择坐标系、选择旋转轴、输入旋转角度，然后创建并显示旋转后的新坐标系。
> 9. 定义了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 10. 定义了ufusr_ask_unload函数，用于卸载NX时立即返回。
>
> 总的来说，这段代码实现了选择坐标系、设置旋转轴和角度、复制坐标系的功能，并且具有撤销功能，是一个典型的NX二次开发示例。
>
