/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"

using namespace std;

template <typename DType, typename LType>
class DataLoader
{
public:
    class Iterator;

private:
    Dataset<DType, LType> *ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int num_batches;
    int seed;
    xt::xarray<int> indices;
    XArrayList<Batch<DType, LType>> batches;

public:
    DataLoader(Dataset<DType, LType> *ptr_dataset, int batch_size,
               bool shuffle = true, bool drop_last = false, int seed = -1)
        : ptr_dataset(ptr_dataset),
          batch_size(batch_size),
          drop_last(drop_last), seed(-1)

    {

        int datasetsize = ptr_dataset->len();
        this->shuffle = shuffle;
        indices = xt::arange<int>(datasetsize); 
        if (shuffle)
        {
            if (seed >= 0)
                xt::random::seed(seed);
            xt::random::shuffle(indices);
        }

        int remainder = datasetsize % batch_size;
        num_batches = datasetsize / batch_size;

        xt::svector<unsigned long> data_shape = ptr_dataset->get_data_shape();

        xt::svector<unsigned long> label_shape = ptr_dataset->get_label_shape();

        xt::xarray<DType> all_data = xt::empty<DType>(data_shape);
        xt::xarray<LType> all_label = xt::empty<LType>(label_shape);

        for (int j = 0; j < datasetsize; j++)
        {
            int data_index = indices[j]; 

            DataLabel<DType, LType> item = ptr_dataset->getitem(data_index);
            view(all_data, j, xt::all()) = item.getData();
            if (ptr_dataset->get_label_shape().size() != 0)
            {
                view(all_label, j, xt::all()) = item.getLabel();
            }
        }
        if (ptr_dataset->get_label_shape().size() == 0)
        {
            all_label = ptr_dataset->getitem(0).getLabel();
        }
      
        for (int i = 0; i < num_batches; i++)
        {
            int first = i * batch_size;
            int last = first + batch_size;

            if (drop_last == false && i == num_batches - 1)
            {
                last = last + remainder;
            }
            if (last > datasetsize)
            {
                last = datasetsize;
            }

            xt::svector<unsigned long> data_batch_shape = data_shape;
            data_batch_shape[0] = last - first;
            xt::xarray<DType> data_batch = xt::empty<DType>(data_batch_shape);

            xt::view(data_batch, xt::all()) = xt::view(all_data, xt::range(first, last));

            if (label_shape.size() != 0)
            {
                xt::svector<unsigned long> label_batch_shape = label_shape;
                label_batch_shape[0] = last - first;
                xt::xarray<LType> label_batch = xt::empty<LType>(label_batch_shape);
                xt::view(label_batch, xt::all()) = xt::view(all_label, xt::range(first, last));
                batches.add(Batch<DType, LType>(data_batch, label_batch));
            }
            else
            {
                xt::xarray<LType> empty_label;
                batches.add(Batch<DType, LType>(data_batch, empty_label));
            }
        }
    }

    virtual ~DataLoader()
    {
    }
    Iterator begin()
    {
        return Iterator(this, 0);
    }

    Iterator end()
    {
        return Iterator(this, num_batches);
    }

    class Iterator
    {
    private:
        int batch_index;
        DataLoader *loader;

    public:
        Iterator(DataLoader<DType, LType> *loader, int batch_index)
            : loader(loader), batch_index(batch_index) {}

        Iterator &operator++()
        {
            ++batch_index;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator!=(const Iterator &other) const
        {
            return batch_index != other.batch_index;
        }

        Batch<DType, LType> &operator*() const
        {
            return loader->batches.get(batch_index); 
        }
    };
};

#endif /* DATALOADER_H */
