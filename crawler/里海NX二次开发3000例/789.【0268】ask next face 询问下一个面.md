### 【0268】ask next face 询问下一个面

#### 代码

```cpp
    static tag_t ask_next_face(tag_t part, tag_t face)  
    {  
        int  
            status,  
            subtype,  
            type;  
        while ((! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face)))  
            && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            status = UF_OBJ_ask_status(face);  
            if ((subtype == UF_solid_face_subtype) && (status == UF_OBJ_ALIVE))  
                return face;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其功能是在NX的部件中遍历所有面，并返回下一个活着的实体面。具体来说，代码首先定义了一个名为ask_next_face的静态方法，接受两个参数part和face。part表示NX部件，face表示一个面。然后，通过循环调用UF_OBJ_cycle_objs_in_part函数，来遍历部件中的所有面。在循环中，通过UF_OBJ_ask_type_and_subtype函数获取每个面的类型和子类型，并通过UF_OBJ_ask_status函数获取其状态。如果面是实体面并且状态为活着，则返回该面。如果遍历完所有面后都没有找到符合条件的面，则返回空标签。因此，这个方法的作用是遍历NX部件中的所有面，并返回下一个活着的实体面。
>
