### 【0257】ask next assorted parts dim 查询下一个混合零件的尺寸

#### 代码

```cpp
    static tag_t ask_next_assorted_parts_dim(tag_t part, tag_t dim)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &dim))  
            && (dim != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(dim, &type, &subtype));  
            if (subtype == UF_dim_assorted_parts_subtype) return dim;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要用于遍历零件中的维度对象，并检查其是否为“assorted_parts”类型。具体步骤如下：
>
> 1. 定义了一个静态函数ask_next_assorted_parts_dim，接受零件标签part和维度标签dim作为参数。
> 2. 在循环中，调用UF_OBJ_cycle_objs_in_part函数遍历零件中的维度对象。每次遍历返回一个新的维度标签，直到遍历结束。
> 3. 每次遍历时，调用UF_OBJ_ask_type_and_subtype函数获取维度对象的类型和子类型。
> 4. 检查维度对象的子类型是否为UF_dim_assorted_parts_subtype。如果是，则返回该维度标签。
> 5. 如果遍历结束仍未找到符合条件的维度，则返回空标签。
> 6. 该函数可以递归调用自身，传入上一次遍历得到的维度标签，继续查找下一个符合条件的维度。
> 7. 主要用于在NX零件中查找“assorted_parts”类型的维度，以进行后续处理。
>
> 这段代码巧妙利用了NX API提供的对象遍历机制，通过递归调用实现了对特定类型维度对象的查找。
>
