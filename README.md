# Enron-Email-Dataset


The [Enron Corpus](https://en.wikipedia.org/wiki/Enron_Corpus) is a large database of over 600,000 emails generated by 158 employees of the Enron Corporation and acquired by the Federal Energy Regulatory Commission during its investigation after the company's collapse.

Enron Email Dataset downloaded from : https://www.cs.cmu.edu/~enron/.  
And it is the __May 7, 2015 Version of dataset__.

Running the `startup.py` file present in this repository will automatically download the tarred and gzipped dataset and extract it for usage.  

***
 The email and finance data is combined into a single dataset, which is explored in this mini-project.

Getting started:

- Clone this [git repository](https://github.com/udacity/ud120-projects)
- Open the starter code from the cloned repo : ```datasets_questions/explore_enron_data.py```

The aggregated _Enron email + financial dataset_ is stored in a dictionary, where each key in the dictionary is a person’s name and the value is a dictionary containing all the features of that person.   
 
The _email + finance (E+F) data dictionary_ is stored as a _pickle file_, which is a handy way to store and load python objects directly. Use ```datasets_questions/explore_enron_data.py``` to load the dataset.

* Number of data points (people) in the dataset - 146
* Features in the Enron Dataset - 21  
* Finding numer of POIs(Person of Interest) in the Enron Data - 18
* Total number of POI's (Obtained from the list of all POI names ```(in ../final_project/poi_names.txt)```) = 35

### Querying the dataset
1.  total value of the stock belonging to James Prentice -- 1095040
2.  Number of email messages from Wesley Colwell to persons of interest -- 11
3.  The value of stock options exercised by Jeffrey K Skilling -- 19250000


The helper functions (```featureFormat()``` and ```targetFeatureSplit()``` in ___tools/feature_format.py__) can take a list of feature names and the data dictionary, and return a numpy array.

***

The folder __My work__ contains the jupyter notebook files showing the dataset's analysis.  
* ```My works/1.Dataset qtns/explore_enron_data_jn.ipynb``` - includes the initial analysis done on the datset.
* ```My works/2.regression/finance_regression_jn.ipynb``` - includes the regression analysis on bonus, salary and long_term_incentive variables of the datset.  
* ``` My works/3.outliers``` - includes codes and Python functions used to remove and check for outliers.  
* ```My works/4. KMeans clustering_feature_scaling``` Deals with feature scaling and clustering.  
* ```My works/5. Text Learning``` itself contains 2 folders - ```1.tools``` having a .ipynb file showing application of user-defined function ```parseOutText()``` and ```2.text learning``` having _vectorize_text_jn.ipynb_ file along with ```.py``` file for function ```parseOutText()```.
* ```My works/6. Feature Selection``` In this chapter, the module's objective was to remove the _signature word_ present in all emails.  
* ```My works/7. PCA``` Finished study on PCA.
* ``````My works/8. Validation`````` 
* ```My works/9. Evaluation metrics``` : 

***

