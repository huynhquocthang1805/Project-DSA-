/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"

#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction) {}

CrossEntropy::CrossEntropy(const CrossEntropy &orig) : ILossLayer(orig) {}

CrossEntropy::~CrossEntropy() {}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t)
{
  // Todo CODE YOUR
  m_aYtarget = t;
  m_aCached_Ypred = X;

  xt::xarray<double> clipped_X = xt::clip(X, 1e-15, 1.0 - 1e-15);
  xt::xarray<double> log_likelihood;
  if (X.dimension() > 1)
  {
    log_likelihood = -xt::sum(t * xt::log(clipped_X), {1});
  }
  else
  {
    log_likelihood = -t * xt::log(clipped_X);
  }
  double loss;
  if (m_eReduction == LossReduction::REDUCE_MEAN)
  {
    loss = xt::mean(log_likelihood)();
  }
  else
  {
    loss = xt::sum(log_likelihood)();
  }

  return loss;
}
xt::xarray<double> CrossEntropy::backward()
{
  // Todo CODE YOUR
  xt::xarray<double> grad = -m_aYtarget / m_aCached_Ypred;

  if (m_eReduction == LossReduction::REDUCE_MEAN)
  {
    return grad / m_aYtarget.shape()[0];
  }
  else
  {
    return grad;
  }
}