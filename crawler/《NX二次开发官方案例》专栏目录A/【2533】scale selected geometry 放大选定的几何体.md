### 【2533】scale selected geometry 放大选定的几何体

#### 代码

```cpp
    /*HEAD SCALE_SELECTED_GEOMETRY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_trns.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
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
        while (((n = select_objects("Select objects to tranform", &objs)) > 0) &&  
            (specify_point("Select invariant scale point", org)) &&  
            (prompt_for_a_number("Enter scale", "Scale", &scale)))  
        {  
            FTN(uf5944)(&one, &scale, org, mx, &resp);  
            ECHO(resp);  
            FTN(uf5947)(mx, objs, &n, &one, &zero, &two, NULL, NULL, &resp);  
            ECHO(resp);  
            UF_free(objs);  
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

> 这段NX二次开发代码主要包括以下功能：
>
> 1. 错误报告函数：用于在函数调用失败时打印错误信息。
> 2. 选择对象函数：用于通过对话框选择对象，并高亮显示所选对象。
> 3. 指定点函数：用于提示用户指定一个点作为缩放的中心点。
> 4. 提示输入数值函数：用于提示用户输入一个数值，如缩放比例。
> 5. 执行变换函数：通过循环调用选择对象、指定中心和输入缩放比例，然后对所选对象进行缩放变换。
> 6. ufusr函数：程序的入口函数，初始化和结束NX，并调用执行变换函数。
> 7. 卸载函数：用于设置程序的卸载方式。
>
> 整个程序的主要逻辑是，通过循环让用户选择对象、指定缩放中心和比例，然后对这些对象进行缩放变换。在函数调用失败时，程序会通过错误报告函数打印错误信息。
>
