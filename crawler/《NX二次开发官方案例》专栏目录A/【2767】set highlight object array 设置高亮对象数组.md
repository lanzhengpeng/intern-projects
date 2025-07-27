### 【2767】set highlight object array 设置高亮对象数组

#### 代码

```cpp
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要实现了设置高亮显示的功能。
>
> 代码中的主要逻辑包括：
>
> 1. 定义了一个静态函数set_highlight_object_array，用于设置多个对象的高亮显示。
> 2. 函数接受3个参数，分别是对象数组objects、数组大小n和标志flag。flag为1时表示高亮显示，为0时表示取消高亮显示。
> 3. 通过循环遍历数组objects中的每一个对象。
> 4. 使用UF_OBJ_ask_type_and_subtype函数获取每个对象的类型和子类型。
> 5. 判断对象类型，如果为实例类型，则使用UF_ASSEM_ask_part_occ_of_inst获取实例的组件，再使用UF_DISP_set_highlight设置高亮；如果为其他类型，则直接使用UF_DISP_set_highlight设置高亮。
> 6. 使用UF_CALL宏调用NX API函数，并检查错误。
> 7. 通过设置标志位flag的值，实现对象的高亮显示或取消高亮显示。
>
> 总的来说，这段代码实现了对NX对象的高亮显示控制，通过遍历对象数组并调用NX API函数来实现。
>
