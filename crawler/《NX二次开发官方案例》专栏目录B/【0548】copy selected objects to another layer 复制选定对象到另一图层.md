### 【0548】copy selected objects to another layer 复制选定对象到另一图层

#### 代码

```cpp
    /*HEAD COPY_SELECTED_OBJECTS_TO_ANOTHER_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 版本中的新功能。 */  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    #ifndef UF_LAYER_MAX_LAYER  
        #define UF_LAYER_MAX_LAYER 256  
    #endif  
    static logical prompt_for_a_layer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            laymsg[100],  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            if ((*number > UF_LAYER_MAX_LAYER) || (*number <= 0))  
            {  
                sprintf(laymsg, "Layers range from 1 to %d", UF_LAYER_MAX_LAYER);  
                uc1601(laymsg, TRUE);  
                return prompt_for_a_layer(prompt, item, number);  
            }  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void layer_copy(tag_t *objects, int n_objects, int to_layer)  
    {  
        int  
            cpy = 2,  
            off = 2,  
            resp;  
        double  
            delta_zero[3] = { 0.0, 0.0, 0.0 },  
            mx[16];  
        tag_t  
            *copies;  
        copies = (tag_t *)UF_allocate_memory(n_objects * sizeof (tag_t), &resp);  
        UF_CALL(resp);  
        FTN(uf5943)(delta_zero, mx);  
        FTN(uf5947)(mx, objects, &n_objects, &cpy, &to_layer, &off, copies, NULL,  
            &resp);  
        WRITENZ(resp);  
        UF_free(copies);  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            to_layer = 2;  
        tag_t  
            *objects;  
        while (((n = select_objects("Select objects to copy", &objects)) > 0)  
            && (prompt_for_a_layer("Layer copy", "TO layer", &to_layer)))  
        {  
            layer_copy(objects, n, to_layer);  
            UF_free(objects);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是将选定的对象复制到指定的图层。
>
> 代码的主要部分包括：
>
> 1. 错误报告函数：report_error用于在函数调用出错时打印错误信息。
> 2. 选择对象函数：select_objects用于通过对话框选择对象，并高亮显示选中的对象。
> 3. 提示输入图层函数：prompt_for_a_layer用于提示用户输入目标图层的编号。
> 4. 整数写入列表窗口函数：write_integer_to_listing_window用于将整数写入列表窗口。
> 5. 图层复制函数：layer_copy用于将一组对象复制到指定的图层。
> 6. 主体函数：do_it用于循环选择对象和输入目标图层，然后调用layer_copy进行复制。
> 7. UF初始化/终止函数：ufusr用于初始化和终止UF，并调用主体函数。
> 8. 询问卸载函数：ufusr_ask_unload用于询问立即卸载。
>
> 通过以上函数的组合，这段代码实现了选择对象复制到指定图层的功能。
>
