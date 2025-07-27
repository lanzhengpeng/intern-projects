### 【0358】ask tag of reference set 查询参考集的标签

#### 代码

```cpp
    static tag_t ask_tag_of_reference_set(tag_t part, char *refset_name)  
    {  
        tag_t  
            refset = NULL_TAG;  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, refset_name,  
            UF_reference_set_type, FALSE, &refset));  
        return refset;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD系统中查询参考集的Tag。以下是代码的详细解释：
>
> 1. 函数名称：ask_tag_of_reference_set功能：查询指定部件(part)中指定名称的参考集(refset_name)的Tag。
> 2. 功能：查询指定部件(part)中指定名称的参考集(refset_name)的Tag。
> 3. 参数说明：part: 指定要查询参考集的部件。refset_name: 指定要查询的参考集的名称。返回值: 查询到的参考集的Tag，如果未找到则返回NULL_TAG。
> 4. part: 指定要查询参考集的部件。
> 5. refset_name: 指定要查询的参考集的名称。
> 6. 返回值: 查询到的参考集的Tag，如果未找到则返回NULL_TAG。
> 7. 函数实现：使用UF_OBJ_cycle_by_name_and_type函数，根据名称和类型在部件中遍历对象。参数说明：part: 指定要遍历的部件。refset_name: 指定要查询的参考集名称。UF_reference_set_type: 指定查询的类型为参考集。FALSE: 表示不查询子装配。&refset: 用于接收查询结果的Tag。如果查询成功，则将查询到的参考集的Tag赋值给refset变量，并返回。如果查询失败，则返回NULL_TAG。
> 8. 使用UF_OBJ_cycle_by_name_and_type函数，根据名称和类型在部件中遍历对象。
> 9. 参数说明：part: 指定要遍历的部件。refset_name: 指定要查询的参考集名称。UF_reference_set_type: 指定查询的类型为参考集。FALSE: 表示不查询子装配。&refset: 用于接收查询结果的Tag。
> 10. part: 指定要遍历的部件。
> 11. refset_name: 指定要查询的参考集名称。
> 12. UF_reference_set_type: 指定查询的类型为参考集。
> 13. FALSE: 表示不查询子装配。
> 14. &refset: 用于接收查询结果的Tag。
> 15. 如果查询成功，则将查询到的参考集的Tag赋值给refset变量，并返回。
> 16. 如果查询失败，则返回NULL_TAG。
> 17. 注意事项：refset_name需要以’\0’结尾。如果部件中没有指定名称的参考集，则返回NULL_TAG。使用UF_OBJ_cycle_by_name_and_type函数时需要包含UF.h头文件。
> 18. refset_name需要以’\0’结尾。
> 19. 如果部件中没有指定名称的参考集，则返回NULL_TAG。
> 20. 使用UF_OBJ_cycle_by_name_and_type函数时需要包含UF.h头文件。
>
> 综上，这段代码实现了在NX部件中根据名称查询参考集Tag的功能，便于进行后续的参考集操作。
>
