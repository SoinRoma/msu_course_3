import numpy as np


class Regression(object):
    def __init__(self, sgd, n_epoch=1, batch_size=16):
        self.W = None
        self.sgd = sgd
        self.n_epoch = n_epoch
        self.batch_size = batch_size

    def fit(self, X, y):
        """Обучение модели.
        Парметры
        X : двумерный массив признаков размера n_samples x n_features
        y : массив/список правильных значений размера n_samples
        Выход
        Метод обучает веса W """

        self.trainX = np.array(X)
        self.trainY = np.array(y)
        self.W = np.zeros(X.shape[1])
        # Для первого раза рандом
        self.W = np.random.rand(self.trainX.shape[1])
        # print( self.W)

        start = 0
        end = self.batch_size

        for i in range(self.n_epoch):
            while end < self.trainX.shape[0]:
                self.W = self.sgd.step(self.trainX[start:end], self.trainY, self.W)
                # print(self.W)
                start += self.batch_size
                end += self.batch_size
            np.random.shuffle(self.trainX)
            start = 0
            end = self.batch_size
            # if end >= self.n_epoch:
            #     start = 0
            #     end = self.batch_size

    def predict(self, X):
        """ Предсказание выходного значения для входных векторов
        Параметры
        X : двумерный массив признаков размера n_samples x n_features
        Выход
        y : Массив размера n_samples"""

        '''
        X = np.array(X)
        y_pred = np.zeros(X.shape[0])
    
        for i in range(X.shape[0]):
            y_pred[i] = np.dot(self.W,X[i])
    
        return y_pred
        '''
        X = np.array(X)
        n = X.shape[0]
        y = np.array([self.W @ X[i] for i in range(n)])

        return y

    def score(self, y_gt, y_pred):
        """Возвращает точность регрессии в виде (1 - u/v),
        где u - суммарный квадрат расхождения y_gt с y_pred,
        v - суммарный квадрат расхождения y_gt с матожиданием y_gt
        Параметры
        y_gt : массив/список правильных значений размера n_samples
        y_pred : массив/список предсказанных значений размера n_samples
        Выход
        accuracy - точность регрессии"""

        difference = y_gt - y_pred
        difference = difference ** 2
        u = np.sum(difference)
        v = y_gt.shape[0] * np.var(y_gt)
        return 1 - u / v


class SGD(object):
    """Класс для реализации метода стохастического градиентного спуска.
    Параметры
    grad : функция вычисления градиента
    alpha : градиентный шаг (default = 1.)"""

    def __init__(self, grad, alpha=1.):
        self.grad = grad
        self.alpha = alpha

    def step(self, X, y, W):
        """Один шаг градиентного спуска.
        Параметры
        X : двумерный массив признаков размера n_samples x n_features
        y : массив/список правильных значений размера n_samples
        W : массив весов размера n_weights
        Выход
        Метод возвращает обновленные веса"""
        trainX = np.array(X)
        k = trainX.shape[0]
        n_weights = np.array(W)
        summa = 0

        for i in range(k):
            summa = summa + self.grad.grad(X[i], y[i], W)

        n_weights = n_weights - self.alpha / k * summa

        return n_weights

        # спросить у Альберта правильно ли ?


class Grad(object):
    """Класс для вычисления градиента по весам от функции потерь.
    Параметры
    loss : функция потерь
    delta : параметр численного дифференцирования (default = 0.000001)"""

    def __init__(self, loss, delta=0.000001):
        self.loss = loss
        self.delta = delta
        pass

    def grad(self, X, y, W):
        """Вычисление градиента.
        Параметры
        X : двумерный массив признаков размера n_samples x n_features
        y : массив/список правильных значений размера n_samples
        W : массив весов размера n_weights
        Выход
        Метод возвращает градиент по весам W в точках X от функции потерь"""

        n_weights = W.size
        ei = np.zeros(n_weights, dtype=float)
        grd = np.zeros(n_weights, dtype=float)
        f = self.loss.val(X, y, W)

        for i in range(n_weights):
            ei[i] = 1
            fi = self.loss.val(X, y, W + ei * self.delta)
            grd[i] = (fi - f) / self.delta
            ei[i] = 0

        return grd


class Loss(object):
    """Класс для вычисления функции потерь.
    Параметры
    l1_coef : коэффициент l1 регуляризации (default = 0)"""

    def __init__(self, l1_coef=0):
        self.l1_coef = l1_coef

    def val(self, X, y, W):
        """Вычисление функции потерь.
        Параметры
        X : двумерный массив признаков размера n_samples x n_features
        y : массив/список правильных значений размера n_samples
        W : массив весов размера n_weights
        Выход
        Метод возвращает значение функции потерь в точках X"""
        """
        n_features = X.shape[0]
        summa = 0
        for i in range(n_features):
            summa += (W @ X[i].T - y[i])**2
        for i in W:
            summa += self.l1_coef * np.abs(i)
        return summa
        """

        L1 = (W @ X - y) ** 2
        L = 0
        for i in W:
            L += np.abs(i)
        L = L * self.l1_coef
        result = L + L1
        return result
