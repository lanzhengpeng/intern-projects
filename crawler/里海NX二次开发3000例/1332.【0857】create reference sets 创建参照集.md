### 【0857】create reference sets 创建参照集

#### 代码

```cpp
    /*HEAD CREATE_REFERENCE_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：UF_print_syslog是在V18中新增的，请只提供译文，不要添加其他无关内容。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    static void ask_wcs_origin(double origin[3])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *members,  
            refset;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        double  
            origin[3],  
            mx[12] = { 1,0,0, 0,1,0, 0,0,1, 0,0,0 };  
        while (prompt_for_text("Enter ref set name", name) &&  
            ((n = select_objects("Select member geometry", &members)) > 0))  
        {  
            /*  Old style call still works  
            UF_CALL(uc5810(name, mx, members, n, &refset));  
            */  
            ask_wcs_origin(origin);  
            UF_CALL(UF_ASSEM_create_ref_set(name, origin, mx, members, n, &refset));  
            UF_free(members);  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含头文件，以调用NX提供的函数库。
> 2. 定义了一个报告错误函数report_error，用于打印函数调用失败时的错误信息。
> 3. 定义了一个提示输入文本函数prompt_for_text，用于获取用户输入的文本。
> 4. 定义了一个选择对象函数select_objects，用于让用户选择几何体作为参考集的成员。
> 5. 定义了一个询问工作坐标系原点函数ask_wcs_origin，用于获取当前工作坐标系的原点。
> 6. 定义了一个创建参考集函数do_it，该函数首先提示用户输入参考集名称，然后选择几何体作为成员，最后调用UF_ASSEM_create_ref_set函数创建参考集。
> 7. 定义了ufusr函数作为程序的入口点，在该函数中初始化NX，调用do_it函数，然后结束NX。
> 8. 定义了卸载函数ufusr_ask_unload，用于卸载用户自定义函数。
>
> 总的来说，这段代码实现了在NX中创建参考集的功能，用户可以通过提示输入参考集名称，选择几何体作为成员，最终创建出参考集。
>
