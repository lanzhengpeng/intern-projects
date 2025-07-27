### 【2028】report component name 报告组件名称

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
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
    static void report_component_name(tag_t component_tag)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        WRITE_D(component_tag);  
        if (!UF_CALL(UF_ASSEM_ask_component_data(component_tag, part_name,  
            refset_name, instance_name, origin, csys_matrix, transform)))  
            WRITE_S(instance_name);  
    }

```

#### 代码解析

> 这段NX二次开发代码用于实现以下功能：
>
> 1. 定义了两个宏WRITE_D和WRITE_S，用于向NX的Listing窗口输出整数和字符串。其中WRITE_D用于输出整数，而WRITE_S用于输出字符串。
> 2. 实现了write_integer_to_listing_window和write_string_to_listing_window两个函数，用于执行具体的输出操作。它们分别调用UF_UI_open_listing_window打开Listing窗口，然后格式化输出内容，并调用UF_UI_write_listing_window写入内容。
> 3. report_component_name函数用于输出组件的信息。它首先获取组件的各种数据（如实例名称、零件名称、坐标系等），然后使用WRITE_D输出组件的tag，接着使用WRITE_S输出组件的实例名称。
> 4. 在整个过程中，使用了UF_CALL宏来简化函数调用的错误检查，确保了代码的健壮性。
> 5. 该代码片段主要用于NX的组件管理，通过输出组件的各种信息，为后续的开发提供了便利。
>
