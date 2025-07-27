### 【2167】report kf list contents 报告KF列表内容

#### 代码

```cpp
    static int report_kf_list_contents(UF_KF_list_p_t list);  
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
                UF_CALL(UF_KF_ask_list(value, &next));  
                report_kf_list_contents(next);  
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
                WRITE("Unknown type\n");  
                break;  
        }  
    }  
    static int report_kf_list_contents(UF_KF_list_p_t list)  
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

```

#### 代码解析

> 这段代码是用于NX Open API中知识融合框架（Knowledge Fusion）的二次开发代码，主要功能是报告知识融合框架中列表的内容。以下是主要函数和功能的介绍：
>
> 1. report_kf_list_contents()- 用于报告知识融合框架中列表的内容。它首先获取列表的元素数量，然后逐个获取列表元素，并调用report_kf_value()函数来报告每个元素的内容，最后释放每个元素的内存。
> 2. report_kf_value()- 用于报告知识融合框架中值对象的内容。它首先获取值的类型，然后根据类型进行不同的处理，如布尔、框架、标签、整数、列表、名称、数字、实例、点、字符串、向量等，并调用不同的宏来打印值的内容。
> 3. WRITE系列宏- 定义了一系列宏，用于打开报告窗口并写入不同类型的数据，如整数、浮点数、字符串、布尔值等。
> 4. write系列函数- 实现了写入不同类型数据到报告窗口的逻辑，如整数、浮点数、字符串等。
> 5. WRITE3F和WRITE16F宏- 用于写入一维和二维浮点数数组到报告窗口。
> 6. UF_KF系列函数- 调用知识融合框架提供的API，用于获取不同类型值的内容。
>
> 总体来说，这段代码实现了对知识融合框架中列表和值对象的遍历和报告功能，通过打开报告窗口并以格式化的方式打印出不同类型值的内容。
>
