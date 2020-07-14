### Optimizing Collision Detection based on OBB Trees Generated with a Genetic Algorithm
_2009_

In this paper a method for generating Oriented Bounding Boxes (OBB) using genetic algorithms (GA) is proposed. OBBs are used in a hierarchy of bounding volumes to detect collisions between objects. Currently, the most used method for generating OBBs is based on the covariance matrix (CV) of the points of the model, but it has many flaws and in most cases does not find the optimum OBB. We performed several tests with both methods (CV vs. GA). Generally the GA method provides a better adjustment of the OBB to the geometry of the models than the CV method. The metric used for measuring the adjustment of the OBB was the volume of the OBB. The time required for performing the collision detection was also tested, obtaining better results with the GA method. It is suggested to use the GA method when the models are rigid and will not be deformed


**Download**
* [SIACG-09](paper/siacg09.pdf)
* [In ResearchGate](https://www.researchgate.net/publication/236834317_Optimizing_Collision_Detection_based_on_OBB_Trees_Generated_with_a_Genetic_Algorithm)


**Slides**

Available in the presentation folder as [presentation/siacg-2009.pdf](presentation/siacg-2009.pdf)


**Notes**

First article written in English.


**How to reference it**

```yaml
@inproceedings{RAM_09_2,
  author       = {Ramírez, Esmitt and Navarro, Héctor and Carmona, Rhadamés and Dos Ramos, José},
  editor       = {F. Serón, O. Rodríguez, J. Rodríguez, E. Coto},
  title        = {Optimizing Collision Detection based on OBB Trees Generated with Genetic Algorithm},
  date         = {2009},
  booktitle    = {Proceedings of the IV Iberoamerican Symposium in Computer Graphics - SIACG (2009)},
  location     = {Venezuela},
  pages        = {1-7},
}
```