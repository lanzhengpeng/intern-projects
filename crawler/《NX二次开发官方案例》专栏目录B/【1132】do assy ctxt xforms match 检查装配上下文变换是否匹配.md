### 【1132】do assy ctxt xforms match 检查装配上下文变换是否匹配

#### 代码

```cpp
    static logical do_assy_ctxt_xforms_match(tag_t one, tag_t two)  
    {  
        tag_t  
            one_occ,  
            two_occ,  
            work_occ = get_work_occurrence();  
        if (one == two) return TRUE; //  matching tags or NULL_TAGs  
        UF_CALL(UF_SO_ask_assy_ctxt_part_occ(one, work_occ, &one_occ));  
        UF_CALL(UF_SO_ask_assy_ctxt_part_occ(two, work_occ, &two_occ));  
        if (one_occ == two_occ) return TRUE;  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于判断两个NX装配上下文是否匹配。以下是代码的主要逻辑：
>
> 1. 如果两个标签相同，则直接返回true，表示匹配。
> 2. 获取当前工作上下文的工作发生。
> 3. 调用UF_SO_ask_assy_ctxt_part_occ函数，获取两个标签在当前工作上下文中的发生。
> 4. 如果两个标签在当前工作上下文中的发生相同，则返回true，表示匹配。
> 5. 如果两个标签在当前工作上下文中的发生不同，则返回false，表示不匹配。
> 6. 如果调用NX API失败，则返回false，表示不匹配。
>
> 总之，这段代码通过比较两个标签在当前工作上下文中的发生，来判断它们是否匹配。如果匹配，则返回true，否则返回false。
>
