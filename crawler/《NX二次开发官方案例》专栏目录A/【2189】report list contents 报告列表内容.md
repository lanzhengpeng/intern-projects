### 【2189】report list contents 报告列表内容

#### 代码

```cpp
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d\n", name,object,type,subtype);  
        }  
        else printf("%s = %d (NULL_TAG)\n", name, object);  
    }  
    static void report_list_contents(char *name, uf_list_p_t list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            item;  
        char  
            item_name[15];  
        UF_CALL(UF_MODL_ask_list_count(list, &n));  
        printf("%s has %d items\n", name, n);  
        for (ii = 0; ii < n; ii++)  
        {  
            sprintf(item_name, "item %d", ii);  
            UF_CALL(UF_MODL_ask_list_item(list, ii, &item));  
            report_tag_type_and_subtype(item_name, item);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要包含两个函数report_tag_type_and_subtype和report_list_contents。
>
> report_tag_type_and_subtype函数用于报告一个对象的名称、tag值、类型和子类型。它首先判断对象是否为NULL_TAG，如果不是，则通过UF_OBJ_ask_type_and_subtype函数获取对象的类型和子类型，并打印出来。
>
> report_list_contents函数用于报告一个列表的名称、项目数量以及每个项目的名称、tag值、类型和子类型。它首先通过UF_MODL_ask_list_count函数获取列表的项目数量，然后遍历列表，通过UF_MODL_ask_list_item函数获取每个项目的tag值，并调用report_tag_type_and_subtype函数报告项目的详细信息。
>
> 这两个函数都使用了NX Open C++ API提供的函数来获取对象和列表的信息，并打印输出。这段代码的目的是为了帮助开发者调试和学习NX Open C++ API，通过打印输出对象和列表的详细信息，让开发者了解API的使用方法和返回值。
>
