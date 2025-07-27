### 【1639】modify color font width of line in a view 修改视图中的线条颜色、字体和宽度

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
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
    static int mask_for_drf_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {{ UF_line_type, 0, 0 },  
                       {UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                       { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_line(char *prompt, UF_DRF_object_p_t line)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_lines, NULL, &resp,  
            &line->object_tag, cp, &line->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(line->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },   
                        { UF_section_segment_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int       
            color = -1,   
            font = 0,   
            width = -1,  
            returnValue = 0;  
        tag_t  
            lineToEdit = NULL_TAG,  
            viewTag = NULL_TAG;  
        double    
            parms[2] = { 0., 1.0 };  
        char  
            viewName[31] = { "" },  
            msg[256] = { "Success!" };  
         lineToEdit = select_a_line("Pick a line:");     
         if(NULL_TAG == lineToEdit) return;          
        UF_CALL(uc1653(viewName));  
        UF_CALL(UF_VIEW_ask_tag_of_view_name(viewName, &viewTag));  
        color = 186;  
        font = UF_OBJ_FONT_DASHED;  
        width = UF_OBJ_WIDTH_THICK;  
        returnValue = uc6406(viewName, lineToEdit, parms, color, font,  width);  
        switch (returnValue)  
        {  
            case 1:  
                strcpy(msg, "View Does Not Exist");  
                break;  
            case 2:  
                strcpy( msg, "Invalid Character In View Name");  
                break;  
            case 3:  
                strcpy( msg, "Invalid Color Number");  
                break;  
            case 4:  
                strcpy( msg, "Invalid Font Number");  
                break;  
            case 5:  
                strcpy( msg, "Invalid Width Number");  
                break;  
            case 6:  
                strcpy( msg, "Invalid Object Type");  
                break;  
            case 7:  
                strcpy( msg, "Data Base Limit ForObject View Mods Full");  
                break;  
            case 8:  
                strcpy( msg, "Attempt To Erase Curve On Full Parameter - Use uc6400 ");  
            default:  
                uc1601(msg, 1);  
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

> 这段代码是一个NX Open的二次开发示例，其主要功能是选择一条线，然后改变其在指定视图中的显示属性。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数ECHO和UF_CALL宏。
> 2. 定义了两个选择过滤函数mask_for_drf_lines和mask_for_lines，用于在选择时过滤出直线对象。
> 3. 定义了选择直线函数select_drf_line和select_a_line，用于选择直线并返回其tag。
> 4. 在do_it函数中，先调用select_a_line选择一条直线，然后获取当前视图的名称和tag，最后调用uc6406函数改变直线在当前视图中的颜色、字体和宽度。
> 5. 在ufusr函数中初始化NX Open，调用do_it函数执行操作，然后终止NX Open。
> 6. 定义了卸载函数ufusr_ask_unload，表示立即卸载。
>
> 总体来说，这段代码通过NX Open的UI、显示、视图等模块，实现了直线选择和显示属性修改的功能。
>
