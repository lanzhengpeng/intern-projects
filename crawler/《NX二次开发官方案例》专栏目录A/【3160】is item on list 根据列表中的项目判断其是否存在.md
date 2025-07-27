### 【3160】is item on list 根据列表中的项目判断其是否存在

#### 代码

```cpp
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的一个函数，用于判断一个项目是否存在于一个项目列表中。以下是代码的详细介绍：
>
> 1. 函数名称：is_item_on_list
> 2. 参数：object_list：项目列表的指针，列表中的每个节点都包含一个项目。item：要判断是否存在于列表中的项目。
> 3. object_list：项目列表的指针，列表中的每个节点都包含一个项目。
> 4. item：要判断是否存在于列表中的项目。
> 5. 返回值：布尔值，表示项目是否存在于列表中。
> 6. 函数逻辑：定义一个整数ii，用于记录循环的次数。定义一个指向项目列表的指针temp，初始指向object_list。使用for循环遍历项目列表，循环条件是temp不为NULL。在循环体中，判断当前节点的eid字段是否等于item，如果相等，则表示项目存在于列表中，返回TRUE。循环结束后，如果项目不存在于列表中，返回FALSE。
> 7. 定义一个整数ii，用于记录循环的次数。
> 8. 定义一个指向项目列表的指针temp，初始指向object_list。
> 9. 使用for循环遍历项目列表，循环条件是temp不为NULL。
> 10. 在循环体中，判断当前节点的eid字段是否等于item，如果相等，则表示项目存在于列表中，返回TRUE。
> 11. 循环结束后，如果项目不存在于列表中，返回FALSE。
> 12. 总结：这个函数通过遍历项目列表，判断指定的项目是否存在于列表中，实现了检查项目是否在列表中的功能。
>
