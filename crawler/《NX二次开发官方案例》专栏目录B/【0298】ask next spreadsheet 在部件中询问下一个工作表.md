### 【0298】ask next spreadsheet 在部件中询问下一个工作表

#### 代码

```cpp
    static tag_t ask_next_spreadsheet(tag_t part, tag_t spreadsheet)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_spreadsheet_type, &spreadsheet));  
        return (spreadsheet);  
    }

```

#### 代码解析

> 这段代码是NX Open API的一个示例，用于遍历NX部件中的所有电子表格对象。具体功能如下：
>
> 1. 函数名为ask_next_spreadsheet，用于在NX部件中逐个获取电子表格对象。
> 2. 函数接收两个参数：part表示NX部件对象，spreadsheet表示当前遍历到的电子表格对象。
> 3. 使用UF_OBJ_cycle_objs_in_part函数遍历part部件中的所有电子表格对象。该函数会遍历当前部件中的所有指定类型的对象，并在每次调用时返回下一个对象。
> 4. UF_OBJ_cycle_objs_in_part的参数包括：part部件对象，UF_spreadsheet_type表示要遍历的对象类型为电子表格，&spreadsheet表示当前遍历到的电子表格对象，函数会修改这个参数的值。
> 5. ask_next_spreadsheet函数返回当前遍历到的电子表格对象。如果已经遍历完所有电子表格，则返回null。
> 6. UF_CALL宏用于调用UF函数，它会检查UF函数的返回码，如果出错会抛出异常。
> 7. 该函数适用于需要遍历和访问NX部件中所有电子表格的场景，如检查、修改或导出电子表格数据等。
>
