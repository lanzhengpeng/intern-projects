### 【1609】map points from selected sketch 从选定的草图映射点

#### 代码

```cpp
    /*HEAD MAP_POINTS_FROM_SELECTED_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sket.h>  
    #include <uf_csys.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog在V18版本中是新增的功能。 */  
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
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    static logical prompt_for_three_numbers(char *prompt, char *item1, char *item2,  
        char * item3, double *number1, double *number2, double *number3)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[3][16];  
        double  
            da[3];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1609(prompt, menu, 3, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void map_point_from_sketch(tag_t sketch, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            mx[12],  
            sketch_mx[9];  
        UF_SKET_info_t  
            sket_info;  
        UF_CALL(UF_SKET_ask_sketch_info(sketch, &sket_info));  
        UF_VEC3_copy(&sket_info.csys[9], &sketch_mx[0]);  
        UF_VEC3_copy(&sket_info.csys[0], &sketch_mx[3]);  
        UF_VEC3_copy(&sket_info.csys[3], &sketch_mx[6]);  
        FTN(uf5940)(sketch_mx, abs_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            point,  
            sketch;  
        double  
            pt[3] = { 0,0,0 };  
        while ((sketch = select_a_sketch("Report csys info")) != NULL_TAG)  
        {  
            while (prompt_for_three_numbers("Map from sketch CSYS", "X", "Y", "Z",  
                &pt[0], &pt[1], &pt[2]))  
            {  
                map_point_from_sketch(sketch, pt);  
                UF_CALL(UF_CURVE_create_point(pt, &point));  
            }  
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

> 这段代码是用于NX的二次开发，主要功能包括：
>
> 1. 提示用户选择一个草图，并获取其坐标系信息。
> 2. 提示用户输入X、Y、Z三个坐标值，将这些值从草图坐标系映射到绝对坐标系。
> 3. 在绝对坐标系下创建一个点，并显示在图形区域中。
> 4. 通过错误处理函数，在出现错误时打印错误信息。
> 5. 通过用户交互，让用户可以不断选择草图并映射坐标点，直到用户取消。
> 6. 在函数 ufusr 中初始化和结束NX API。
> 7. 在 ufusr_ask_unload 中，设置立即卸载二次开发程序。
>
> 整体来说，这段代码通过交互式方式，使用户可以方便地将草图坐标系中的点映射到绝对坐标系中，并显示出来。
>
