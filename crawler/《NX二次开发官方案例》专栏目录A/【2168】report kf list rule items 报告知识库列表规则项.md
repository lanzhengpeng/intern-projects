### 【2168】report kf list rule items 报告知识库列表规则项

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_part.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%.3f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static int report_kf_list_contents_a(UF_KF_list_p_t list);  
    static void report_kf_value(UF_KF_value_p_t value)  
    {  
        logical  
            boolean;  
        unsigned int  
            name;  
        int  
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
        if (UF_CALL(UF_KF_ask_type(value, &type))) return;  
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
                UF_CALL(UF_KF_ask_list(value, &next));  
                report_kf_list_contents_a(next);  
                UF_CALL(UF_KF_free_list_object_contents(next));  
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
                ECHO("%s", "Unknown ");  
                WRITE_D(type);  
                break;  
        }  
    }  
    static int report_kf_list_contents_a(UF_KF_list_p_t list)  
    {  
        int  
            ii,  
            count = 0;  
        UF_KF_value_p_t  
            element;  
        if (list != NULL) UF_CALL(UF_KF_ask_list_count(list, &count));  
        WRITE_D(count);  
        for (ii = 1; ii <= count; ii++)  
        {  
            WRITE_D(ii);  
            UF_CALL(UF_KF_ask_list_item(list, ii, &element));  
            report_kf_value(element);  
            UF_CALL(UF_KF_free_rule_value(element));  
        }  
        return count;  
    }  
    static int report_kf_list_contents_b(UF_KF_list_p_t list)  
    {  
        int  
            ii,  
            count = 0;  
        UF_KF_list_p_t  
            sub_list;  
        UF_KF_list_p_t  
            next = list;  
        UF_KF_value_p_t  
            element;  
        UF_KF_type_t  
            type;  
        if (list != NULL) UF_CALL(UF_KF_ask_list_count(list, &count));  
        WRITE_D(count);  
        for (ii = 1; ii <= count; ii++)  
        {  
            WRITE_D(ii);  
            UF_CALL(UF_KF_list_pop(next, &element, &next));  
            UF_CALL(UF_KF_ask_type(element, &type));  
            WRITE_D(type);  
            if (type == UF_KF_list)  
            {  
                UF_CALL(UF_KF_ask_list(element, &sub_list));  
                report_kf_list_contents_b(sub_list);  
            }  
            UF_CALL(UF_KF_free_rule_value(element));  
        }  
        return count;  
    }  
    static int report_kf_list_contents_c(UF_KF_value_p_t list_value)  
    {  
        int  
            count =0,  
            ii;  
        char  
            *data_type,  
            *value_str;  
        UF_KF_list_p_t  
            list;  
        UF_KF_value_p_t  
            sub_list;  
        if (list_value != NULL) UF_CALL(UF_KF_ask_list(list_value, &list));  
        UF_CALL(UF_KF_ask_list_count(list, &count));  
        WRITE_D(count);  
        for (ii = 1; ii <= count; ii++)  
        {  
            UF_CALL(UF_KF_ask_value_of_list_item(list_value, ii, &value_str,  
                &data_type, &sub_list));  
            WRITE_D(ii);  
            WRITE_S(value_str);  
            UF_free(value_str);  
            WRITE_S(data_type);  
            UF_free(data_type);  
            if (sub_list != NULL)  
            {  
                report_kf_list_contents_c(sub_list);  
                UF_CALL(UF_KF_free_rule_value(sub_list));  
            }  
        }  
        UF_CALL(UF_KF_free_list_object_contents(list));  
        return count;  
    }  
    /* qq3123197280 */  
    DllExport extern void list_report_a(UF_KF_value_t *data,  
                                      int num,  
                                      UF_KF_value_t *rv)  
    {  
        int  
            count = 0;  
        UF_KF_list_p_t  
            list;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_KF_ask_list(data[0], &list));  
        ECHO("\n%s\n", "List Report A:");  
        count = report_kf_list_contents_a(list);  
        UF_CALL(UF_KF_free_list_object_contents(list));  
        UF_CALL(UF_KF_make_integer(count, rv));  
        UF_CALL(UF_terminate());  
    }  
    /* qq3123197280 */  
    DllExport extern void list_report_b(UF_KF_value_t *data,  
                                      int num,  
                                      UF_KF_value_t *rv)  
    {  
        int  
            count = 0;  
        UF_KF_list_p_t  
            list;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_KF_ask_list(data[0], &list));  
        ECHO("\n%s\n", "List Report B:");  
        count = report_kf_list_contents_b(list);  
        UF_CALL(UF_KF_free_list_object_contents(list));  
        UF_CALL(UF_KF_make_integer(count, rv));  
        UF_CALL(UF_terminate());  
    }  
    /* qq3123197280 */  
    DllExport extern void list_report_c(UF_KF_value_t *data,  
                                      int num,  
                                      UF_KF_value_t *rv)  
    {  
        int  
            count = 0;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        ECHO("\n%s\n", "List Report C:");  
        count = report_kf_list_contents_c(data[0]);  
        UF_CALL(UF_KF_make_integer(count, rv));  
        UF_CALL(UF_terminate());  
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
        UF_KF_list_p_t  
            list;  
        UF_KF_value_p_t  
            list_object;  
        while (prompt_for_text("Enter list rule name chain", name_chain))  
            if (!UF_CALL(UF_KF_ask_list_of_instance(name_chain, &list_object)))  
            {  
                UF_CALL(UF_KF_ask_list(list_object, &list));  
                ECHO("\n%s\n", "Method a:");  
                report_kf_list_contents_a(list);  
                ECHO("\n%s\n", "Method b:");  
                report_kf_list_contents_b(list);  
                ECHO("\n%s\n", "Method c:");  
                report_kf_list_contents_c(list_object);  
                UF_CALL(UF_KF_free_list_object_contents(list));  
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

> 这段代码是NX Open C++ API编写的NX二次开发示例代码，主要功能包括：
>
> 1. 定义了ECHO、WRITE_D等宏，用于在列表窗口输出信息。
> 2. 定义了report_error函数，用于报告错误信息。
> 3. 定义了report_kf_value函数，用于报告知识框架(KF)值的类型和内容。
> 4. 定义了report_kf_list_contents_a/b/c函数，用于报告KF列表的内容，分别采用不同的方式。
> 5. 定义了list_report_a/b/c函数，分别调用report_kf_list_contents_a/b/c函数，并以KF整数形式返回列表元素个数。
> 6. 定义了do_it函数，用于提示用户输入KF规则名链，然后报告KF列表内容。
> 7. 定义了ufusr函数，初始化NX环境后调用do_it函数，然后终止NX环境。
> 8. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 总体来说，这段代码提供了查询和报告NX知识框架列表内容的示例，使用了不同的方法，具有一定的参考价值。
>
