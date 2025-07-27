### 【1042】delete specified component set 删除指定的组件集

#### 代码

```cpp
    /*HEAD DELETE_SPECIFIED_COMPONENT_SET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的提示，UF_print_syslog 是在 V18 版本中新增的功能。在翻译时，请确保翻译准确，不要添加任何废话，只需回答UF_print_syslog在V18版本中是新增的功能即可。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_component_set(tag_t part, tag_t cset)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_set_type, &cset));  
        return (cset);  
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
    static void do_it(void)  
    {  
        logical  
            done = FALSE;  
        int  
            cnt;  
        tag_t  
            cset,  
            part = UF_PART_ask_display_part();  
        char  
            cset_name[UF_OBJ_NAME_LEN + 1] = { "" };  
        do  
        {  
            WRITE("component sets in part\n");  
            cset = NULL_TAG;  
            cnt = 0;  
            while ((cset = ask_next_component_set(part, cset)) != NULL_TAG)  
            {  
                cnt++;  
                WRITE_D(cset);  
                UF_CALL(UF_OBJ_ask_name(cset, cset_name));  
                WRITE_S(cset_name);  
            }  
            if ((cnt > 0) && prompt_for_text("Delete component set", cset_name))  
            {  
                UF_UI_close_listing_window();  
                UF_CALL(UF_OBJ_cycle_by_name_and_type(part, cset_name,  
                    UF_component_set_type, FALSE, &cset));  
                WRITE_D(cset);  
                if (cset != NULL_TAG)  
                {  
                    WRITE("deleting - ");  
                    WRITE_S(cset_name);  
                    UF_CALL(UF_OBJ_delete_object(cset));  
                }  
                WRITE("\n");  
            }  
            else done = TRUE;  
        } while (!done);  
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

> 这段NX二次开发代码主要实现以下功能：
>
> 1. 遍历当前打开的部件中的所有组件集，并输出每个组件集的名称和ID。
> 2. 提示用户输入要删除的组件集名称，然后根据名称找到对应的组件集ID。
> 3. 删除用户指定的组件集。
> 4. 通过错误处理函数report_error，在发生错误时输出错误信息。
> 5. 使用宏WRITE系列输出信息到日志窗口。
> 6. 定义了组件集的遍历函数ask_next_component_set。
> 7. 定义了获取用户输入文本的函数prompt_for_text。
> 8. 主函数do_it实现了上述功能。
> 9. ufusr函数是NX二次开发的标准入口函数，在这里调用do_it。
> 10. ufusr_ask_unload函数用于在卸载用户自定义函数时立即卸载，而不是等待。
>
> 总体而言，这段代码通过交互式提示用户删除指定的组件集，提高了NX软件的用户友好性。
>
