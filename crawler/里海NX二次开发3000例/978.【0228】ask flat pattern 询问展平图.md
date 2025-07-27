### 【0228】ask flat pattern 询问展平图

#### 代码

```cpp
    /*HEAD ASK_FLAT_PATTERN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_smd.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的函数。 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            material_side,  
            n_add_curves,  
            n_faces,  
            n_lines;  
        tag_t  
            *add_curves,  
            body = NULL_TAG,  
            *faces,  
            fp = NULL_TAG,  
            *lines,  
            part = UF_PART_ask_display_part();  
        char  
            *bend_formula;  
        double  
            angle,  
            bend_allowance,  
            inside_radius,  
            thickness;  
        if (!UF_CALL(UF_SMD_initialize()))  
        {  
            while ((body = ask_next_body(part, body)) != NULL_TAG)  
            {  
                WRITE_D(body);  
                if (!UF_CALL(UF_SMD_ask_flat_pattern(body, &fp)) &&  
                    (fp != NULL_TAG))  
                {  
                    UF_CALL(UF_SMD_ask_contour_lines(fp, &n_lines, &lines));  
                    WRITE_D(n_lines);  
                    if (n_lines) UF_free(lines);  
                    UF_CALL(UF_SMD_ask_bend_tan_lines(fp, &n_lines, &lines));  
                    WRITE_D(n_lines);  
                    if (n_lines) UF_free(lines);  
                    UF_CALL(UF_SMD_ask_fp_faces(body, &n_faces, &faces));  
                    WRITE_D(n_faces);  
                    if (n_faces) UF_free(faces);  
                    UF_CALL(UF_SMD_ask_fp_add_curves(body, &n_add_curves,  
                        &add_curves));  
                    WRITE_D(n_add_curves);  
                    if (n_add_curves > 0) UF_free(add_curves);  
                    UF_CALL(UF_SMD_ask_bend_lines(fp, &n_lines, &lines));  
                    WRITE_D(n_lines);  
                    for (ii = 0; ii < n_lines; ii++)  
                    {  
                        if (!UF_CALL(UF_SMD_ask_bend_line_data(lines[ii],  
                            &bend_formula, &angle, &inside_radius, &thickness,  
                            &bend_allowance, &material_side)))  
                        {  
                            WRITE_S(bend_formula);  
                            UF_free(bend_formula);  
                            WRITE_F(angle);  
                            WRITE_F(inside_radius);  
                            WRITE_F(thickness);  
                            WRITE_F(bend_allowance);  
                            WRITE_D(material_side);  
                        }  
                    }  
                    if (n_lines) UF_free(lines);  
                }  
            }  
            UF_CALL(UF_SMD_terminate());  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了写整数、浮点数和字符串到列表窗口的函数，用于输出调试信息。
> 3. 定义了获取零件中下一个实体的函数ask_next_body。
> 4. 在主函数do_it中，遍历零件中的实体，获取每个实体的平面展开信息，并输出到列表窗口。
> 5. 输出了平面展开的轮廓线、折弯线、附加曲线、折弯线数据和材料侧等信息。
> 6. 在ufusr函数中初始化和终止NX API，并调用主函数do_it。
> 7. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码通过遍历零件中的实体，获取每个实体的平面展开信息，并将这些信息输出到列表窗口，实现了NX零件的平面展开查询功能。
>
