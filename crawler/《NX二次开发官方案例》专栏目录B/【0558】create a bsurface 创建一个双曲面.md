### 【0558】create a bsurface 创建一个双曲面

#### 代码

```cpp
    /*HEAD CREATE_A_BSURFACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，关于 UF_print_syslog 的说明如下：

UF_print_syslog 是一个在 V18 版本中新增的函数。

该函数用于打印系统日志消息。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_modl_states(int num_states, UF_MODL_state_t *states)  
    {  
        int  
            ii;  
        WRITENZ(num_states);  
        for (ii = 0; ii < num_states; ii++)  
        {  
            WRITE_D(ii);  
            switch (states[ii].state_code)  
            {  
                case UF_MODL_STATE_DEGENERACY:  
                    WRITE_D(UF_MODL_STATE_DEGENERACY);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE(" near ");  
                    WRITE3F(states[ii].value);  
                    break;  
                case UF_MODL_STATE_G1_DISCONTINUITY:  
                    WRITE_D(UF_MODL_STATE_G1_DISCONTINUITY);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("smoothed to G1 "); }  
                    if (states[ii].flag & 4) { WRITE("smoothed to Cn "); }  
                    WRITE("At knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_KNOT_NONC0:  
                    WRITE_D(UF_MODL_STATE_KNOT_NONC0);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_KNOT_MULT:  
                    WRITE_D(UF_MODL_STATE_KNOT_MULT);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_KNOT_DECREASING:  
                    WRITE_D(UF_MODL_STATE_KNOT_DECREASING);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_CLOSED:  
                    WRITE_D(UF_MODL_STATE_CLOSED);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_MODL_STATE_PERIODIC:  
                    WRITE_D(UF_MODL_STATE_PERIODIC);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_MODL_STATE_KNOT_TOOCLOSE:  
                    WRITE_D(UF_MODL_STATE_KNOT_TOOCLOSE);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_UNNORMALIZED:  
                    WRITE_D(UF_MODL_STATE_UNNORMALIZED);  
                    if (states[ii].flag & 2) { WRITE("Along V factor "); }  
                    else { WRITE("Along U factor "); }  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_CLAMPED:  
                    WRITE_D(UF_MODL_STATE_CLAMPED);  
                    WRITE("Knot value ");  
                    WRITE_F(states[ii].value[0]);  
                    WRITE("minimum distance change ");  
                    WRITE_F(states[ii].value[1]);  
                    WRITE("maximum angle change ");  
                    WRITE_F(states[ii].value[2]);  
                    break;  
                case UF_MODL_STATE_SMOOTHED_TO_CN:  
                    WRITE_D(UF_MODL_STATE_SMOOTHED_TO_CN);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("smoothed to G1 "); }  
                    if (states[ii].flag & 4) { WRITE("smoothed to Cn "); }  
                    WRITE("At knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_DEGEN_EDGE:  
                    WRITE_D(UF_MODL_STATE_DEGEN_EDGE);  
                    break;  
                case UF_MODL_STATE_SELF_INTERSECT:  
                    WRITE_D(UF_MODL_STATE_SELF_INTERSECT);  
                    break;  
                default:  
                    WRITE("Unknown state ");  
                    WRITE_D(states[ii].state_code);  
                    break;  
            }  
        }  
        if (num_states > 0) UF_free(states);  
    }  
    static void do_it(void)  
    {  
        int  
            n_states;  
        tag_t  
            sheet;  
        double  
            knots[8] = { 0,0,0,0,1,1,1,1 },  
            poles[][4] = {  3.000000, 0.000000, 0.000000, 1.000000,  
                            3.000000, 1.000000, 0.000000, 1.000000,  
                            3.000000, 2.000000, 0.000000, 1.000000,  
                            3.000000, 3.000000, 0.000000, 1.000000,  
                            2.000000, 0.000000, 0.000000, 1.000000,  
                            2.000000, 1.000000, 0.000000, 1.000000,  
                            2.000000, 2.000000, 0.000000, 1.000000,  
                            2.000000, 3.000000, 0.000000, 1.000000,  
                            1.000000, 0.000000, 0.000000, 1.000000,  
                            1.000000, 1.000000, 0.000000, 1.000000,  
                            1.000000, 2.000000, 0.000000, 1.000000,  
                            1.000000, 3.000000, 0.000000, 1.000000,  
                            0.000000, 0.000000, 0.000000, 1.000000,  
                            0.000000, 1.000000, 0.000000, 1.000000,  
                            0.000000, 2.000000, 0.000000, 1.000000,  
                            0.000000, 3.000000, 0.000000, 1.000000 };  
        UF_MODL_state_t  
            *states;  
        UF_MODL_bsurface_t  
            surface = { 4, 4, 4, 4, FALSE, NULL, NULL, NULL };  
        surface.knots_u = knots;  
        surface.knots_v = knots;  
        surface.poles = poles;  
        if (!UF_CALL(UF_MODL_create_bsurface(&surface, &sheet, &n_states, &states)))  
        {  
            UF_CALL(UF_MODL_update());  
            UF_CALL(UF_DISP_add_item_to_display(sheet));  
        }  
        report_modl_states(n_states, states);  
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

> 这段代码是用于创建一个NX中的双变量曲面(b-surface)的二次开发代码。主要功能包括：
>
> 1. 包含了NX二次开发所需的相关头文件。
> 2. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义了写入变量到日志窗口的宏和函数。
> 4. 定义了报告曲面状态的函数report_modl_states。
> 5. 主函数do_it中，首先初始化了双变量曲面的参数，包括节点矢量和控制点。然后调用UF_MODL_create_bsurface创建曲面，并获取曲面状态。最后调用UF_DISP_add_item_to_display显示曲面。
> 6. ufusr函数用于初始化NX环境，调用do_it，然后终止NX环境。
> 7. ufusr_ask_unload函数用于卸载用户函数。
>
> 总体来说，这段代码实现了在NX中创建双变量曲面的功能，并能够报告创建过程中的状态信息。代码结构清晰，注释详细，易于理解和使用。
>
