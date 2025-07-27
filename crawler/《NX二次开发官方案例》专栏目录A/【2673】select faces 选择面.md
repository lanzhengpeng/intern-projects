### 【2673】select faces 选择面

#### 代码

```cpp
    /*HEAD SELECT_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。

因此，翻译为：

UF_print_syslog是V18版本新增的函数，只回答译文，不要废话。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *faces;  
        while ((n = select_faces("Select faces", &faces)) != 0)  
        {  
            for (ii = 0; ii < n; ii++) WRITE_D(faces[ii]);  
            UF_free(faces);  
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

> 这段代码是一个NX二次开发示例，用于选择和打印NX中的面。主要包括以下功能：
>
> 1. 包含必要的NX头文件，定义了UF_CALL宏，用于简化UF函数调用和错误处理。
> 2. 定义了report_error函数，用于处理UF函数调用错误，输出错误信息到系统日志窗口。
> 3. 定义了write_integer_to_listing_window函数，用于将整数打印到列表窗口。
> 4. 定义了mask_for_faces函数，设置选择掩码，使得只能选择实体面。
> 5. 定义了select_faces函数，弹出一个对话框，让用户选择面，并返回面的数量和数组指针。
> 6. do_it函数循环调用select_faces，每次选择后打印面的tag，然后释放面数组内存。
> 7. ufusr函数是NX二次开发的入口函数，在这里调用do_it函数执行主体功能。
> 8. ufusr_ask_unload函数返回立即卸载，表示二次开发应用程序可以立即卸载。
>
> 总的来说，这段代码实现了选择NX中的面并打印面tag的功能，使用了NX提供的UI和UF接口。
>
