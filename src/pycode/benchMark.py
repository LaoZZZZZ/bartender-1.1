#!/usr/bin/python
from time import time
from sklearn  import metrics
from sklearn import cross_validation
from sklearn.utils.extmath  import density
###############################################################################
# Benchmark classifiers
def benchmark(clf,X_train,y_train,X_test,y_test,crossValidationcategories =None,report=True):
    print('_' * 80)
    print("Training: ")
    print(clf)
    t0 = time()
    clf.fit(X_train, y_train)
    train_time = time() - t0
    print("train time: %0.3fs" % train_time)

    t0 = time()
    pred = clf.predict(X_test)
    test_time = time() - t0
    print("test time:  %0.3fs" % test_time)

    score = metrics.accuracy_score(y_test, pred)
    print("accuracy:   %0.3f" % score)

    if hasattr(clf, 'coef_'):
        print("dimensionality: %d" % clf.coef_.shape[1])
        print("density: %f" % density(clf.coef_))
	'''
        if opts.print_top10 and feature_names is not None:
            print("top 10 keywords per class:")
            for i, category in enumerate(categories):
                top10 = np.argsort(clf.coef_[i])[-10:]
                print(trim("%s: %s"
                      % (category, " ".join(feature_names[top10]))))
        print()
    if report:
        print("classification report:")
        print(metrics.classification_report(y_test, pred,
                                            target_names=categories))
    if opts.print_cm:
        print("confusion matrix:")
        print(metrics.confusion_matrix(y_test, pred))
	'''
    clf_descr = str(clf).split('(')[0]
    return clf_descr, score, train_time, test_time

if __name__ == '__main__':
	pass	
