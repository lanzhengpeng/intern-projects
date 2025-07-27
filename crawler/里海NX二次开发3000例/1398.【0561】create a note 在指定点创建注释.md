### 【0561】create a note 在指定点创建注释

#### 代码

```cpp
    /*HEAD CREATE_A_NOTE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能。 */  
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
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    static void do_it(void)  
    {  
        tag_t  
                a_note;  
        double  
                loc[3];  
        char  
                text[1][133];  
        while (specify_point("Indicate point for note", loc))  
        {  
            map_abs2wcs(loc);  
            sprintf(&text[0][0], "%f, %f, %f", loc[0], loc[1], loc[2]);  
            uc5540(1, text, loc, &a_note);  
            uc1601("Note created", 1);  
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

> 这段代码是一个NX二次开发示例，用于创建注释。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了一个获取用户指定点的函数specify_point。
> 3. 定义了一个坐标系转换函数map_abs2wcs，用于将绝对坐标系转换成工作坐标系。
> 4. 定义了主函数do_it，其中循环调用specify_point获取点，然后转换坐标系，并调用NX API函数uc5540创建注释。
> 5. 定义了NX的UF初始化和终止函数ufusr。
> 6. 定义了一个卸载函数ufusr_ask_unload。
>
> 代码通过NX提供的UF和UI API，实现了获取用户输入、坐标系转换、创建注释等功能。通过错误报告函数，可以方便地调试代码。整体结构清晰，是一个典型的NX二次开发示例。
>
