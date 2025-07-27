### 【2922】split bodies and delete selected resultant bodies 将实体分割并删除选定的结果实体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static int select_bodies(char *prompt, tag_t **bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp, &cnt,  
            bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*bodies)[ii], FALSE));  
        return cnt;  
    }  
    /* qq3123197280 */  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_a_face_plane_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_face_plane_datums, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            num_bodies,  
            num_split_bodies;  
        tag_t  
            *bodies,  
            cutting_body,  
            *split_bodies;  
        while ((cutting_body = select_a_face_plane_datum(  
                "Select faces, planes, datums to split with")) &&  
                (num_bodies = select_bodies("Select bodies", &bodies)))  
        {  
            UF_CALL(UF_MODL_split_body (num_bodies, bodies, cutting_body,  
                &num_split_bodies, &split_bodies));  
            ECHO("Split created %d bodies\n", num_split_bodies);  
            for (ii = 0; ii < num_split_bodies; ii++)  
            {  
                UF_DISP_set_highlight(split_bodies[ii], TRUE);  
                if(ask_yes_or_no("resulting bodies","Delete this body?"))  
                    UF_CALL(UF_OBJ_delete_object(split_bodies[ii]));  
                else  
                    UF_CALL(UF_DISP_set_highlight(split_bodies[ii], FALSE));  
            }  
            UF_free(split_bodies);  
            UF_free(bodies);  
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

> 这是NX Open C++的一个二次开发示例代码，主要功能如下：
>
> 1. 定义了一个ECHO函数，用于在列表窗口和系统日志中打印信息。
> 2. 定义了UF_CALL宏，用于调用NX Open C++的函数，并报告错误。
> 3. 定义了mask_for_bodies函数，用于设置选择体时的过滤条件。
> 4. 定义了ask_yes_or_no函数，用于弹出Yes/No对话框询问用户。
> 5. 定义了select_bodies函数，用于弹出选择对话框，让用户选择多个体。
> 6. 定义了mask_for_face_plane_datums函数，用于设置选择面/平面/基准面时的过滤条件。
> 7. 定义了select_a_face_plane_datum函数，用于弹出选择对话框，让用户选择一个面/平面/基准面。
> 8. 定义了do_it函数，循环执行以下步骤：选择一个面/平面/基准面作为切割体选择多个体作为要切割的体调用UF_MODL_split_body函数进行布尔分割输出分割生成的体数量高亮显示分割生成的每个体，并询问用户是否删除如果用户选择删除，则调用UF_OBJ_delete_object函数删除该体
> 9. 选择一个面/平面/基准面作为切割体
> 10. 选择多个体作为要切割的体
> 11. 调用UF_MODL_split_body函数进行布尔分割
> 12. 输出分割生成的体数量
> 13. 高亮显示分割生成的每个体，并询问用户是否删除
> 14. 如果用户选择删除，则调用UF_OBJ_delete_object函数删除该体
> 15. 定义了ufusr函数，作为程序的入口，初始化NX Open C++环境，调用do_it函数执行主体功能，最后终止NX Open C++环境。
> 16. 定义了ufusr_ask_unload函数，用于卸载用户自定义应用程序。
>
> 综上，这段代码实现了NX Open C++的布尔分割功能，让用户可以交互式地选择切割面和被切割的体，进行布尔分割，并支持删除分割生成的体。
>
