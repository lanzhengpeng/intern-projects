### 【0999】delete a category 删除一个分类

#### 代码

```cpp
    static void delete_a_category(char *cat_name)  
    {  
        tag_t  
            cat = NULL_TAG;  
        cat = cycle_by_name_and_type(cat_name, UF_layer_category_type, cat);  
        if (cat != NULL_TAG) UF_CALL(UF_OBJ_delete_object(cat));  
    }

```

#### 代码解析

> 根据代码，这是段NX Open C++二次开发代码，主要功能是删除NX中的某个图层分类(category)。具体步骤如下：
>
> 1. 定义一个静态函数delete_a_category，该函数接收一个字符指针参数cat_name，表示要删除的图层分类名称。
> 2. 在函数内部，定义一个tag_t类型的变量cat，用于存储图层分类的tag，并初始化为NULL_TAG。
> 3. 调用cycle_by_name_and_type函数，根据分类名称cat_name和类型UF_layer_category_type，获取该分类的tag，并存储在cat变量中。
> 4. 判断cat是否不为NULL_TAG，如果找到该分类，则调用UF_OBJ_delete_object函数，传入cat参数，删除该图层分类对象。
> 5. 如果没有找到该分类，cat为NULL_TAG，则不做任何操作。
> 6. 函数结束。
>
> 该代码实现了根据分类名称删除NX图层分类的功能，可帮助清理无用的分类，提高NX图纸的整洁度。
>
