### 【0409】build component partname from db attributes 构建组件的部件名称从数据库属性

#### 代码

```cpp
    static void build_component_partname_from_db_attributes(tag_t component,  
        char *partname)  
    {  
        char  
            str[UF_ATTR_MAX_STRING_LEN+1];  
        UF_ATTR_value_t  
            value;  
        strcpy(partname, "@DB/");  
        value.value.string = str;  
        UF_CALL(UF_ATTR_read_value(component,"DB_PART_NO",UF_ATTR_string,&value));  
        strcat(partname, value.value.string);  
        UF_CALL(UF_ATTR_read_value(component,"DB_PART_REV",UF_ATTR_string,&value));  
        strcat(partname, "/");  
        strcat(partname, value.value.string);  
    }

```

#### 代码解析

> 这段代码是一个用于NX（Siemens NX）的二次开发代码，其主要功能是从NX部件的属性中构建部件的名称。
>
> 代码流程如下：
>
> 1. 定义一个名为build_component_partname_from_db_attributes的静态函数，该函数接受NX部件的tag_t和部件名称的char指针作为参数。
> 2. 在函数内部，定义一个长度为UF_ATTR_MAX_STRING_LEN+1的字符数组str，用于存储读取的字符串属性值。
> 3. 定义一个UF_ATTR_value_t类型的变量value，用于存储属性值。设置其value.string字段指向str数组。
> 4. 使用strcpy函数将部件名称前缀"@DB/"复制到partname字符串中。
> 5. 通过UF_ATTR_read_value函数读取NX部件的"DB_PART_NO"字符串属性值，并追加到partname字符串中。
> 6. 继续使用UF_ATTR_read_value函数读取NX部件的"DB_PART_REV"字符串属性值，并在partname字符串中添加"/"和该属性值。
> 7. 最终，partname字符串将包含NX部件的完整名称，格式为"@DB/DB_PART_NO/DB_PART_REV"。
> 8. 函数调用结束。
>
> 通过这段代码，可以实现从NX部件的数据库属性中提取并构建出部件的完整名称，这对于在二次开发中获取和操作NX部件信息非常有用。
>
