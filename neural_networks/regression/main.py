import numpy as np
from regression import *

from tests import generate_regression_data, test_regression_model

alpha = 0.005
delta = 1e-8
l1_coef = 0
n_epoch = 20
batch_size = 40

trainX, trainY, testX, testY = generate_regression_data(Nfeat=100, Mtrain=100, Mtest=2000)

loss = Loss(l1_coef)
grad = Grad(loss, delta)
sgd = SGD(grad, alpha)
reg = Regression(sgd, n_epoch, batch_size)


test_regression_model(reg, trainX, trainY, testX, testY)

