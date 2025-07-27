### 【1664】move view to another drawing 将视图移动到另一个绘图

#### 代码

```cpp
    /*HEAD MOVE_VIEW_TO_ANOTHER_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    static tag_t select_drawing_member_view(void)  
    {  
        tag_t  
            view_tag;  
        double  
            rr2[3];  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        if (uc1615("Select member view", rr2) == 5)  
        {  
            uc1653(view_name);  
            if (!UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag)))  
                return view_tag;  
        }  
        return NULL_TAG;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_tag_of_drawing(char *dwg_name)  
    {  
        tag_t  
            dwg = NULL_TAG;  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_by_name(dwg_name, &dwg)) && dwg)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(dwg, &type, &subtype));  
            if (type == UF_drawing_type) return dwg;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            drawing_tag,  
            view_tag;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1] = { "SH2" };  
        while (((view_tag = select_drawing_member_view()) != NULL_TAG) &&  
            (prompt_for_text("Enter drawing name", dname)))  
            if ((drawing_tag = ask_tag_of_drawing(dname)) != NULL_TAG)  
                UF_CALL(UF_DRAW_move_view_to_drawing(view_tag, drawing_tag));  
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

> 这段代码是一个NX Open C++的二次开发示例，实现了将一个绘图成员视图移动到另一个绘图的功能。以下是该代码的主要功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息，包括文件名、行号、函数调用和错误码等。
> 2. 选择绘图成员视图函数：定义了一个选择绘图成员视图的函数select_drawing_member_view，使用UI函数让用户选择一个视图，并返回该视图的tag。
> 3. 提示输入文本函数：定义了一个提示输入文本的函数prompt_for_text，用于获取用户输入的文本。
> 4. 查询绘图tag函数：定义了一个根据绘图名称查询绘图tag的函数ask_tag_of_drawing，使用对象循环函数查询绘图对象。
> 5. 移动视图到另一个绘图函数：定义了主函数do_it，用于不断提示用户选择视图和输入目标绘图名称，然后调用UF_DRAW_move_view_to_drawing函数将视图移动到目标绘图。
> 6. ufusr入口函数：定义了ufusr入口函数，在初始化后调用do_it函数，最后进行终止。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总体来说，这段代码实现了NX中视图的移动功能，用户可以通过选择视图和输入目标绘图名称，将视图从一个绘图移动到另一个绘图。
>
