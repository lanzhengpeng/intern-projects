### 【2531】scale selected assorted parts drafting entities 缩放选定杂项绘图实体

#### 代码

```cpp
    /*HEAD SCALE_SELECTED_ASSORTED_PARTS_DRAFTING_ENTITIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_trns.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
注意：UF_print_syslog 是在 V18 中新增的，请只提供翻译，不要添加任何废话。 */  
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
    static int mask_for_draft_assorted_parts(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_assorted_parts_subtype, 0};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_draft_assorted_parts(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select draft assorted parts",  
            prompt, UF_UI_SEL_SCOPE_WORK_PART, mask_for_draft_assorted_parts,  
            NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], FALSE));  
        return cnt;  
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
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            one = 1,  
            resp,  
            two = 2,  
            zero = 0;  
        tag_t  
            *objs;  
        double  
            mx[12],  
            org[3],  
            scale = { 1.0 };  
        while (((n = select_draft_assorted_parts("Select finish symbols to scale",  
            &objs)) > 0) && (prompt_for_a_number("Enter scale", "Scale", &scale)))  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_DRF_ask_origin(objs[ii], org));  
                FTN(uf5944)(&one, &scale, org, mx, &resp);  
                FTN(uf5947)(mx, objs, &n, &one, &zero, &two, NULL, NULL, &resp);  
            }  
            UF_free(objs);  
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

> 这段代码是一个NX二次开发示例，其主要功能是选择草图符号，然后对其进行缩放。以下是代码的主要内容和步骤：
>
> 1. 包含必要的NX头文件，如uf.h、uf_ui.h等。
> 2. 定义一个错误报告函数report_error，用于在调用NX API失败时输出错误信息。
> 3. 定义一个选择草图符号的函数mask_for_draft_assorted_parts，它设置选择条件为只选择草图符号。
> 4. 定义一个选择草图符号的函数select_draft_assorted_parts，它使用NX选择对话框让用户选择草图符号，并返回所选对象数组。
> 5. 定义一个提示用户输入缩放比例的函数prompt_for_a_number，它使用NX的菜单对话框让用户输入一个数值。
> 6. 在主函数do_it中，循环调用select_draft_assorted_parts和prompt_for_a_number，获取用户选择的草图符号和缩放比例，然后调用NX的缩放函数进行缩放。
> 7. 定义ufusr函数，它是NX二次开发的入口函数，在这里初始化NX环境，调用do_it，然后结束NX环境。
> 8. 定义一个卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来说，这段代码实现了选择草图符号，提示用户输入缩放比例，然后对选择的符号进行缩放的功能，是NX二次开发的一个简单示例。
>
