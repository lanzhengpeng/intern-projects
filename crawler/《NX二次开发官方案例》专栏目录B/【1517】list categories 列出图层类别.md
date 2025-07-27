### 【1517】list categories 列出图层类别

#### 代码

```cpp
    static void list_categories(void)  
    {  
        int  
            j = 0;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1],  
            descr[MAX_LINE_SIZE+1];  
        tag_t  
            cat = NULL_TAG,  
            part;  
        UF_LAYER_category_info_t  
            cat_info;  
        cat_info.name = name;  
        cat_info.descr = descr;  
        part = UF_PART_ask_display_part();  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_layer_category_type, &cat));  
            if (cat)  
            {  
                UF_CALL(UF_LAYER_ask_category_info(cat, &cat_info));  
                if (!j) printf("Layer Categories:\n");  
                printf("  %d.  %s - %s\n", ++j, cat_info.name, cat_info.descr);  
            }  
        } while (cat);  
    }

```

#### 代码解析

> 这段代码是用于列出NX中的图层分类的二次开发代码。以下是该代码的主要功能：
>
> 1. 定义了所需的变量，包括图层分类的名称、描述和tag，以及用于循环的变量。
> 2. 使用UF_PART_ask_display_part()获取当前显示的零件。
> 3. 使用do-while循环遍历零件中的所有图层分类对象。
> 4. 在循环中，使用UF_OBJ_cycle_objs_in_part()函数获取当前遍历到的图层分类对象，并检查是否为NULL。
> 5. 如果获取到非NULL的图层分类对象，则使用UF_LAYER_ask_category_info()获取该分类的名称和描述信息。
> 6. 输出分类的名称和描述信息，其中分类的序号是通过j变量进行控制的。
> 7. 循环直到遍历完所有的图层分类对象。
>
> 总的来说，这段代码实现了遍历并输出当前零件中所有图层分类的名称和描述的功能，有助于用户了解当前零件中的图层分类情况。
>
