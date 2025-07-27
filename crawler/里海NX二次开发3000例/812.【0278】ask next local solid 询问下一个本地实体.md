### 【0278】ask next local solid 询问下一个本地实体

#### 代码

```cpp
    static tag_t ask_next_local_solid(tag_t part, tag_t solid)  
    {  
        int  
            subtype,  
            status,  
            type;  
        while ((solid = UF_OBJ_cycle_all(part, solid)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(solid, &type, &subtype));  
            status = UF_OBJ_ask_status(solid);  
            if ((type == UF_solid_type) &&  
                (subtype == UF_solid_body_subtype) &&  
                ((status == 3) || (UF_OBJ_is_object_a_promotion(solid))) &&  
                (!UF_ASSEM_is_occurrence(solid))) return solid;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX中遍历零件体并获取下一个本地实体的函数。其具体功能如下：
>
> 1. 遍历零件体：通过UF_OBJ_cycle_all函数遍历指定零件体part中的所有实体，以获取实体tag。
> 2. 获取实体类型和子类型：使用UF_OBJ_ask_type_and_subtype函数获取实体的类型和子类型，并判断是否为实体类型（UF_solid_type）和实体体子类型（UF_solid_body_subtype）。
> 3. 判断实体状态：使用UF_OBJ_ask_status函数判断实体的状态，确保其处于“显示”状态（状态为3）。
> 4. 检查是否为推广实体：通过UF_OBJ_is_object_a_promotion函数检查实体是否为推广实体。
> 5. 判断是否为实例：使用UF_ASSEM_is_occurrence函数判断实体是否为实例。
> 6. 返回满足条件的实体tag：如果实体满足上述所有条件，则返回该实体的tag，否则继续遍历下一个实体。
> 7. 遍历结束返回null：当遍历完所有实体后，若没有找到符合条件的实体，则返回NULL_TAG。
>
> 综上所述，这段代码的作用是在NX中遍历指定零件体，并获取下一个本地实体，该实体需要是实体的显示状态、非实例，并且不是推广实体。
>
