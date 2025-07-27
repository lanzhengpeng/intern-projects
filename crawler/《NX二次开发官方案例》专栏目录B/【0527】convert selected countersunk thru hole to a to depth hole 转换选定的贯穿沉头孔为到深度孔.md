### 【0527】convert selected countersunk thru hole to a to depth hole 转换选定的贯穿沉头孔为到深度孔

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static void set_expression_tag_value(tag_t exp, double new_value)  
    {  
        char  
            *exp_str,  
            *lhs_str,  
            new_exp[UF_MAX_EXP_LENGTH+1],  
            *rhs_str;  
        UF_CALL(UF_MODL_ask_exp_tag_string(exp, &exp_str));  
        UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs_str, &rhs_str, &exp));  
        sprintf(new_exp, "%s=%f", lhs_str, new_value);  
        strip_trailing_zeros(new_exp);  
        UF_CALL(UF_MODL_edit_exp(new_exp));  
        UF_free(exp_str);  
        UF_free(rhs_str);  
        UF_free(lhs_str);  
    }  
    static void do_it(void)  
    {  
        int  
            thru_flag;  
        tag_t  
            exp,  
            hole;  
        double  
            value;  
        char  
            *csink_angle,  
            *depth,  
            *dia1,  
            *dia2,  
            *lhs,  
            *rhs,  
            *tip_angle;  
        while ((hole = select_a_feature("Select a Countersunk Hole")) != NULL_TAG)  
        {  
            if (UF_CALL(UF_MODL_ask_c_sunk_hole_parms(hole, 0, &dia1, &dia2,  
                &depth, &csink_angle, &tip_angle, &thru_flag))) continue;  
            ECHO("Before:\n");  
            WRITE_S(dia1);  
            WRITE_S(dia2);  
            WRITE_S(depth);  
            WRITE_S(csink_angle);  
            WRITE_S(tip_angle);  
            WRITE_L(thru_flag);  
            UF_free(dia1);  
            UF_free(dia2);  
            UF_free(depth);  
            UF_free(csink_angle);  
            UF_free(tip_angle);  
            if (thru_flag)  
            {  
                UF_CALL(UF_MODL_remove_thru_faces(hole));  
                UF_CALL(UF_MODL_edit_hole_type(hole, UF_COUNTER_SUNK_HOLE));  
                UF_CALL(UF_MODL_update());  
                UF_CALL(UF_MODL_ask_c_sunk_hole_parms(hole, 0, &dia1, &dia2,  
                    &depth, &csink_angle, &tip_angle, &thru_flag));  
                UF_CALL(UF_MODL_dissect_exp_string(depth, &lhs, &rhs, &exp));  
                UF_free(lhs);  
                UF_free(rhs);  
                UF_CALL(UF_MODL_ask_exp_tag_value(exp, &value));  
                prompt_for_a_number("New value", "Depth", &value);  
                set_expression_tag_value(exp, value);  
                UF_CALL(UF_MODL_dissect_exp_string(tip_angle, &lhs, &rhs, &exp));  
                UF_free(lhs);  
                UF_free(rhs);  
                UF_CALL(UF_MODL_ask_exp_tag_value(exp, &value));  
                prompt_for_a_number("New value", "Tip Angle", &value);  
                set_expression_tag_value(exp, value);  
                UF_free(dia1);  
                UF_free(dia2);  
                UF_free(depth);  
                UF_free(csink_angle);  
                UF_free(tip_angle);  
                UF_CALL(UF_MODL_update());  
                UF_CALL(UF_MODL_ask_c_sunk_hole_parms(hole, 0, &dia1, &dia2,  
                    &depth, &csink_angle, &tip_angle, &thru_flag));  
                ECHO("After:\n");  
                WRITE_S(dia1);  
                WRITE_S(dia2);  
                WRITE_S(depth);  
                WRITE_S(csink_angle);  
                WRITE_S(tip_angle);  
                WRITE_L(thru_flag);  
                UF_free(dia1);  
                UF_free(dia2);  
                UF_free(depth);  
                UF_free(csink_angle);  
                UF_free(tip_angle);  
            }  
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

> 这段NX二次开发代码的主要功能是选择模型中的沉头孔特征，修改其参数，并更新模型。
>
> 代码首先包含必要的NX Open头文件，并定义了错误报告函数report_error和输出函数ECHO。
>
> select_a_feature函数用于选择模型中的一个沉头孔特征。
>
> strip_trailing_zeros函数用于去除字符串尾部的零。
>
> set_expression_tag_value函数用于修改表达式标签的值。
>
> do_it函数是主函数，其逻辑如下：
>
> 1. 循环选择模型中的沉头孔特征，直到没有更多特征为止。
> 2. 获取当前沉头孔的参数，并输出。
> 3. 如果沉头孔是贯通的，则移除其贯通面，修改为沉头孔类型，并更新模型。
> 4. 获取沉头孔的深度和尖角参数表达式，提示用户修改这两个参数。
> 5. 更新沉头孔的参数，并输出修改后的参数。
> 6. 重复以上步骤，直到所有沉头孔都被处理。
>
> ufusr函数是NX调用的主函数，用于初始化NX、执行do_it函数，以及终止NX。
>
> ufusr_ask_unload函数用于卸载NX Open库。
>
> 总的来说，这段代码通过交互式选择和修改沉头孔参数，实现了NX模型的二次开发。
>
