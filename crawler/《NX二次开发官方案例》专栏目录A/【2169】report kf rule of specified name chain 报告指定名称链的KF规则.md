### 【2169】report kf rule of specified name chain 报告指定名称链的KF规则

#### 代码

```cpp
    /*HEAD REPORT_KF_RULE_OF_SPECIFIED_NAME_CHAIN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18版本中新增了UF_print_syslog函数。 */  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
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
    static void report_kf_value(UF_KF_value_p_t value)  
    {  
        logical  
            boolean;  
        unsigned int  
            name;  
        int  
            count,  
            ii,  
            integer;  
        tag_t  
            ug_object;  
        double  
            frame[16],  
            number,  
            point[3],  
            vector[3];  
        char  
            *name_string,  
            *string;  
        UF_KF_instance_p_t  
            instance;  
        UF_KF_list_p_t  
            next;  
        UF_KF_type_t  
            type;  
        UF_KF_value_p_t  
            element;  
        if (UF_CALL(UF_KF_ask_type(value, &type))) return;  
        WRITE_D(type);  
        switch (type)  
        {  
            case UF_KF_null:  
                WRITE_D(UF_KF_null);  
                break;  
            case UF_KF_error_type:  
                WRITE_D(UF_KF_error_type);  
                break;  
            case UF_KF_any:  
                WRITE_D(UF_KF_any);  
                break;  
            case UF_KF_boolean:  
                WRITE_D(UF_KF_boolean);  
                UF_CALL(UF_KF_ask_boolean(value, &boolean));  
                WRITE_L(boolean);  
                break;  
            case UF_KF_defeval:  
                WRITE_D(UF_KF_defeval);  
                break;  
            case UF_KF_frame:  
                WRITE_D(UF_KF_frame);  
                UF_CALL(UF_KF_ask_frame(value, frame));  
                WRITE16F(frame);  
                break;  
            case UF_KF_tag:  
                WRITE_D(UF_KF_tag);  
                UF_CALL(UF_KF_ask_tag(value, &ug_object));  
                WRITE_D(ug_object);  
                break;  
            case UF_KF_integer:  
                WRITE_D(UF_KF_integer);  
                UF_CALL(UF_KF_ask_integer(value, &integer));  
                WRITE_D(integer);  
                break;  
            case UF_KF_list:  
                WRITE_D(UF_KF_list);  
                UF_CALL(UF_KF_ask_list_count(value, &count));  
                WRITE_D(count);  
                UF_CALL(UF_KF_ask_list(value, &next));  
                for (ii = 1; ii <= count; ii++)  
                {  
                    WRITE_D(ii);  
                    UF_CALL(UF_KF_list_pop(next, &element, &next));  
                    report_kf_value(element);  
                    UF_CALL(UF_KF_free_rule_value(element));  
                }  
                break;  
            case UF_KF_marker:  
                WRITE_D(UF_KF_marker);  
                break;  
            case UF_KF_name:  
                WRITE_D(UF_KF_name);  
                UF_CALL(UF_KF_ask_name(value, &name));  
                UF_CALL(UF_KF_ask_string_of_name(name, &name_string));  
                WRITE_S(name_string);  
                UF_free(name_string);  
                break;  
            case UF_KF_no_value:  
                WRITE_D(UF_KF_no_value);  
                break;  
            case UF_KF_number:  
                WRITE_D(UF_KF_number);  
                UF_CALL(UF_KF_ask_number(value, &number));  
                WRITE_F(number);  
                break;  
            case UF_KF_instance:  
                WRITE_D(UF_KF_instance);  
                UF_CALL(UF_KF_ask_instance(value, &instance));  
                UF_CALL(UF_KF_ask_ug_object(instance, &ug_object));  
                WRITE_D(ug_object);  
                break;  
            case UF_KF_point:  
                WRITE_D(UF_KF_point);  
                UF_CALL(UF_KF_ask_point(value, point));  
                WRITE3F(point);  
                break;  
            case UF_KF_string:  
                WRITE_D(UF_KF_string);  
                UF_CALL(UF_KF_ask_string(value, &string));  
                WRITE_S(string);  
                UF_free(string);  
                break;  
            case UF_KF_vector:  
                WRITE_D(UF_KF_vector);  
                UF_CALL(UF_KF_ask_vector(value, vector));  
                WRITE3F(vector);  
                break;  
            case UF_KF_user:  
                WRITE_D(UF_KF_user);  
                break;  
            default:  
                WRITE("Unknown type\n");  
                break;  
        }  
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
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            name_chain[133] = { "" };  
        UF_KF_value_p_t  
            value;  
        UF_CALL(UF_KF_init_part(part));  
        while (prompt_for_text("Enter list rule name chain", name_chain))  
            if (!UF_CALL(UF_KF_evaluate_rule(name_chain, &value)))  
            {  
                WRITE_S(name_chain);  
                report_kf_value(value);  
                UF_CALL(UF_KF_free_rule_value(value));  
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

> 根据代码内容和注释，这是一个NX Open C++ API编写的NX二次开发代码，主要用于查询和打印NX知识融合(Knowledge Fusion)规则链的值。
>
> 主要功能包括：
>
> 1. 定义了错误报告宏，用于捕获并打印API调用错误。
> 2. 定义了一系列宏，用于将不同类型的值打印到列表窗口。
> 3. 定义了一个函数，用于递归打印Knowledge Fusion值的类型和内容。
> 4. 定义了一个函数，用于提示用户输入规则链名称。
> 5. 主函数do_it()首先获取当前显示的零件，然后循环提示用户输入规则链名称，并查询该规则链的值，然后打印到列表窗口。
> 6. 定义了ufusr()函数，这是NX Open C++程序的入口函数，用于初始化并调用主函数。
> 7. 定义了ufusr_ask_unload()函数，用于卸载插件。
>
> 总的来说，这是一个用于查询和打印Knowledge Fusion规则链值的NX Open C++插件。
>
