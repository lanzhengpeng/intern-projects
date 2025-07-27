### 【2469】rotate copy selected csys about a specified axis 选择一个坐标系，围绕指定轴旋转并复制

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
    #include <uf_vec.h>  
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
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
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
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    static void do_it(void)  
    {  
        int  
            which_axis = 1;  
        tag_t  
            csys = NULL_TAG,  
            mx,  
            new_csys;  
        double  
            delta_degrees = 180,  
            new_axes[9],  
            new_origin[3],  
            old_axes[9],  
            old_origin[3],  
            rotation_mx[9],  
            to_new[3],  
            to_old[3],  
            vector[3],  
            vector_origin[3];  
        while (((csys = select_a_csys("Rotate/copy")) != NULL_TAG) &&  
            specify_vector("Rotate about", vector, vector_origin) &&  
            prompt_for_a_number("Rotate how far?", "Degrees", &delta_degrees))  
        {  
            set_undo_mark("Rotate/copy csys");  
            UF_MTX3_rotate_about_axis(vector, delta_degrees*DEGRA, rotation_mx);  
            ask_csys_info(csys, old_origin, old_axes);  
            UF_VEC3_sub(old_origin, vector_origin, to_old);  
            UF_MTX3_vec_multiply(to_old, rotation_mx, to_new);  
            UF_VEC3_add(vector_origin, to_new, new_origin);  
            UF_MTX3_multiply_t(rotation_mx, old_axes, new_axes);  
            UF_CALL(UF_CSYS_create_matrix(new_axes, &mx));  
            UF_CALL(UF_CSYS_create_csys(new_origin, mx, &new_csys));  
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

> 这段代码是一段用于NX的二次开发代码，其主要功能是选择一个坐标系，指定一个旋转轴和角度，然后创建一个新的坐标系，该坐标系是原坐标系按照指定轴旋转指定角度后的结果。
>
> 主要步骤包括：
>
> 1. 定义了一些错误报告和日志输出函数，以便于调试。
> 2. 提供了一个函数用于选择坐标系，并限定只能选择坐标系类型的对象。
> 3. 提供了一个函数用于指定一个向量。
> 4. 提供了一个函数用于提示用户输入一个数字。
> 5. 提供了一个函数用于获取坐标系的原点和轴矩阵。
> 6. 提供了一个函数用于在撤销树中设置一个标记。
> 7. 在主函数中，循环执行以下步骤：选择一个坐标系指定旋转轴和角度获取原坐标系的参数计算新坐标系的原点和轴矩阵创建并高亮显示新坐标系
> 8. 选择一个坐标系
> 9. 指定旋转轴和角度
> 10. 获取原坐标系的参数
> 11. 计算新坐标系的原点和轴矩阵
> 12. 创建并高亮显示新坐标系
> 13. 最后，提供了卸载函数，用于在卸载时立即清理。
>
> 总体来说，这段代码实现了选择坐标系、指定旋转轴和角度、创建新坐标系的功能，对于坐标系操作非常有用。
>
