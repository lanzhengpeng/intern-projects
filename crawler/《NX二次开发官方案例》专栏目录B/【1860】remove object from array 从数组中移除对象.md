### 【1860】remove object from array 从数组中移除对象

#### 代码

```cpp
    static int remove_object_from_array(tag_t del_obj, tag_t *objects, int n_objs)  
    {  
        int  
            ii;  
    /*  Find the index of the object being removed in the array 译:找到数组中被移除对象的索引。 */  
        for (ii = 0; ((ii < n_objs) && (objects[ii] != del_obj)); ii++);  
    /*  If found, shuffle up to fill in the space where that object was 译:如果找到，将其他对象随机填充到该对象原本所在的位置。 */  
        if (ii < n_objs)  
        {  
            n_objs--;  
            for (; ii < n_objs; ii++) objects[ii] = objects[ii+1];  
        }  
        return n_objs;  
    }

```

#### 代码解析

> 这段代码是一个名为remove_object_from_array的函数，用于从对象数组中移除指定对象，并返回移除后数组中剩余对象的数量。该函数的主要步骤如下：
>
> 1. 遍历对象数组，寻找要移除的对象的索引。
> 2. 如果找到要移除的对象，则将数组中该对象之后的所有对象向前移动一位，以覆盖该对象的位置。
> 3. 修改数组中对象的数量。
> 4. 返回修改后数组中对象的数量。
>
> 具体来说，代码首先使用一个for循环遍历数组，寻找要移除的对象。一旦找到，就退出循环，并得到要移除对象的索引ii。接着判断ii是否小于数组中对象的数量n_objs，如果小于，则表示找到了要移除的对象。然后，将数组中ii之后的所有对象向前移动一位，覆盖掉要移除的对象。移动完成后，修改数组中对象的数量n_objs减1。最后返回修改后的n_objs。
>
> 该函数通过移动数组元素的方式移除指定对象，避免了内存的重新分配，效率较高。
>
