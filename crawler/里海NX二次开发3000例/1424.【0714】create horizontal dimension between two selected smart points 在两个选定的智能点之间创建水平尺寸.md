### 【0714】create horizontal dimension between two selected smart points 在两个选定的智能点之间创建水平尺寸

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_so.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static logical construct_point(char *prompt, tag_t *pt, tag_t *view)  
    {  
        int  
            ii,  
            n_parents,  
            resp = 0,  
            subtype,  
            type;  
        tag_t  
            *parents;  
        double  
            loc[3] = { 0,0,0 };  
        UF_UI_POINT_base_method_t  
            method = UF_UI_POINT_INFERRED;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE + 1];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        if (UF_CALL(UF_UI_point_construct(prompt, &method, pt, loc, &resp))  
            || (resp != UF_UI_OK))  
            return false;  
        else  
        {  
            UF_CALL(uc1653(view_name));  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, view));  
    //  When working on a drawing, UF_UI_point_construct returns a smart point on  
    //  the drawing which has been extracted from another smart point in modeling.  
    //  The dimension needs to go to the one in modeling to show the correct value,  
    //  so use the parent point rather than the one returned by UF_UI_point_construct.  
            UF_CALL(UF_SO_ask_parents(*pt, UF_SO_ASK_ALL_PARENTS, &n_parents,  
                &parents));  
            for (ii = 0; ii < n_parents; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(parents[ii], &type, &subtype));  
                if (type == UF_point_type)  
                {  
                    *pt = parents[ii];  
                    break;  
                }  
            }  
            UF_free(parents);  
            return true;  
        }  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int resp;  
        UF_OBJ_disp_props_t  
            color = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_UI_set_cursor_view(TRUE));  
        resp = uc1615(prompt, pos);  
        if (resp == 5)  
        {  
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_CURSOR,  
                pos, &color, UF_DISP_ASTERISK));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            dim;  
        double  
            d_pos[3],  
            m_pos[3],  
            mpr[70],  
            origin[3];  
        UF_DRF_object_t  
            object[2];  
        UF_DRF_text_t  
            text = { NULL, 0, NULL };  
        char  
            diameter[7],  
            radius[7];  
        UF_CALL(UF_DRF_init_object_structure(&object[0]));  
        UF_CALL(UF_DRF_init_object_structure(&object[1]));  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
        mpi[0] = 1; /* automatic 译:自动的 */  
        mpi[7] = 1; /* Only automatic text 译:好的，我会只提供自动翻译的文本。 */  
        mpi[30] = 5; /* Mid-center 译:Mid-center翻译为“中心”。 */  
        mpr[32] = 0.5;  /*  DIMENSION TEXT SIZE 译:尺寸文本大小 */  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
        while (  
            (construct_point("Select 1st point", &object[0].object_tag,  
                &object[0].object_view_tag)) &&  
            (construct_point("Select 2st point", &object[1].object_tag,  
                &object[1].object_view_tag)) &&  
            (select_pos("Select origin", origin)))  
        {  
            UF_CALL(UF_CURVE_ask_point_data(object[0].object_tag, m_pos));  
            UF_CALL(UF_VIEW_map_model_to_drawing(object[0].object_view_tag, m_pos,  
                d_pos));  
            object[0].assoc_dwg_pos[0] = d_pos[0];  
            object[0].assoc_dwg_pos[1] = d_pos[1];  
            UF_CALL(UF_CURVE_ask_point_data(object[1].object_tag, m_pos));  
            UF_CALL(UF_VIEW_map_model_to_drawing(object[1].object_view_tag, m_pos,  
                d_pos));  
            object[1].assoc_dwg_pos[0] = d_pos[0];  
            object[1].assoc_dwg_pos[1] = d_pos[1];  
            UF_CALL(UF_DRF_create_horizontal_dim(&object[0], &object[1], &text,  
                origin, &dim));  
        }  
    }  
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

> 根据代码，这是一段基于NX的二次开发代码，其主要功能是创建一个水平尺寸标注。下面是代码的主要功能介绍：
>
> 1. 定义了错误报告宏UF_CALL，用于检测NX函数调用是否成功，如果失败，则打印错误信息。
> 2. 定义了ECHO宏，用于在日志窗口和系统日志中输出信息。
> 3. 定义了WRITE_S函数，用于在日志窗口输出字符串信息。
> 4. 定义了construct_point函数，用于构造一个点对象，并返回其tag。
> 5. 定义了select_pos函数，用于选择一个位置点。
> 6. 定义了do_it函数，这是主函数，用于创建水平尺寸标注。
> 7. 在do_it函数中，首先初始化尺寸标注对象，并设置尺寸标注的参数。
> 8. 使用循环，连续调用construct_point和select_pos函数，获取两个点和原点。
> 9. 调用UF_DRF_create_horizontal_dim函数创建水平尺寸标注。
> 10. 在ufusr函数中，初始化NX，调用do_it函数，然后终止NX。
> 11. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总的来说，这段代码通过交互式选择两个点和原点，创建了一个水平尺寸标注。
>
