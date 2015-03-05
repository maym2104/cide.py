/****************************************************************************
 *  Librairie:    LibZoneTransit                                            *
 *  Auteur:       Mariane Maynard                                           *
 *  Description:  Construction de la librairie boost python de la classe    *
 *                ZoneTransit, Modification et Fichier                      *
 ****************************************************************************/

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/overloads.hpp>
#include "ZoneTransit.h"
#include "SFichier.h"
#include "Fichier.h"
#include "Ajout.h"
#include "Suppression.h"
#include "Chronometre.h"
#include <string>
#include "Types.h"

using namespace boost::python;
using std::string;
using boost::shared_ptr;
using namespace types;

BOOST_PYTHON_FUNCTION_OVERLOADS(minuter_overloads, minuter, 1, 2)

BOOST_PYTHON_MODULE(libZoneTransit)
{
  //Definit la classe Fichier. Fichier est un alias pour FichierType<string>
  class_<Fichier, boost::noncopyable>("File")
    .def(init<const string &>())
    .def("writeToDisk", &Fichier::ecrireSurDisque)
    .def("insert", &Fichier::inserer)
    .def("delete", &Fichier::supprimer)
    .def("printContent", &Fichier::printContenu)
    .add_property("content", &Fichier::getContenu);

  class_<ZoneTransit, boost::noncopyable>("TransitZone")
    .def(init<const string&>())
    .def("add", &ZoneTransit::add)
    .def("writeModifications", &ZoneTransit::ecrireModifications)
    .def("isEmpty", &ZoneTransit::estVide)
    .add_property("content", &ZoneTransit::getContenu);

  //Definit la classe Modification (non instantiable, abstraite) et le type ModificationPtr
  class_<Modification, boost::noncopyable, ModificationPtr>("Modification", no_init)
  //add_property ajoute un attribut, auquel on peut spécifier une fonction get et une fonction set (utilise comme get et set en c#)
  //dans ce cas, je specifie seulement le get, donc les attributs sont publics en read only seulement
    .add_property("position", &Modification::getPosition)
    .add_property("size", &Modification::getTaille)
    .def("apply", &Modification::effectuerModification)
    .def("update", &Modification::mettreAJour);

  //Definit Ajout heritant de Modification
  class_<Ajout, bases<Modification>>("Addition")
    .def(init<pos_t, size_t, const string&>())
    .def(init<pos_t, const string&>())
    .def(init<const Ajout&>())
    .add_property("data", &Ajout::getData);

  class_<Suppression, bases<Modification>>("Removal")
    .def(init<pos_t, size_t>())
    .def(init<const Suppression&>());

  class_<std::vector<ModificationPtr> >("Modifications")
    .def(vector_indexing_suite<std::vector<ModificationPtr>, true >());
/*
  class_<Chronometre>("Timer")
    .def("set", &Chronometre::mesurer)
    .def("reset", &Chronometre::reset)
    .def("time", &Chronometre::minuter, minuter_overloads())
    .staticmethod("time");*/
}
