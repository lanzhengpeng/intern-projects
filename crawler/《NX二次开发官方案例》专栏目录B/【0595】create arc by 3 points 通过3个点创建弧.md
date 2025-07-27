### 【0595】create arc by 3 points 通过3个点创建弧

#### 代码

```cpp
    /*HEAD CREATE_ARC_BY_3_POINTS  CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                sprintf(err, "error %d at line %d in %s\n", irc, line, file);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n\n");  
            }  
            else  
            {  
                fprintf(stderr, "%s\n", err);  
                fprintf(stderr, "error %d at line %d in %s\n", irc, line, file);  
                fprintf(stderr, "%s;\n\n", call);  
    #ifdef _USRDLL  
                uc1601(err, TRUE);  
                sprintf(err, "error %d at line %d in %s\n", irc, line, file);  
                uc1601(err, TRUE);  
    #endif  
            }  
        }  
        return(irc);  
    }  
    static logical ask_arc_or_circle(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
    //    return (2 - resp);  
        return (resp);  
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
    static void do_it(void)  
    {  
        double point[3][3];  
        tag_t arc;  
        int type;  
        while(   
            specify_point("Select Point 1", point[0]) &&  
            specify_point("Select Point 2", point[1]) &&  
            specify_point("Select Point 3", point[2]) )  
        {  
            type = ask_arc_or_circle( "Arc or Circle", "Create arc only?");  
            printf(" type = %d\n", type);  
            UF_CALL( UF_CURVE_create_arc_thru_3pts (  
                type, /*1=arc, 2=circle*/  
                point[0],   
                point[1],   
                point[2],   
                &arc ) );  
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

> 这是NX的二次开发代码，其主要功能是通过3个点创建圆弧或圆。
>
> 主要代码逻辑如下：
>
> 1. 错误报告函数：report_error()用于报告函数调用错误，并在列表窗口或控制台中输出错误信息。
> 2. 询问函数：ask_arc_or_circle()用于询问用户创建圆弧还是圆，返回1表示圆弧，2表示圆。
> 3. 点指定函数：specify_point()用于提示用户指定一个点，并将点的坐标存储在参数loc中。
> 4. 主函数：do_it()中循环调用specify_point()函数，让用户指定3个点，然后调用UF_CURVE_create_arc_thru_3pts()函数根据3个点创建圆弧或圆。
> 5. ufusr函数：ufusr()是NX二次开发的标准入口函数，在这里调用do_it()函数。
> 6. 卸载函数：ufusr_ask_unload()返回立即卸载标志。
>
> 通过这段代码，用户可以方便地通过指定3个点来创建圆弧或圆，实现了NX圆弧/圆的交互式创建功能。
>
