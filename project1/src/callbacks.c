#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "gestiondeparking.h"
#include "Reservation.h"
#define COLUMN_REGION 0
#define COLUMN_ANNEE  1
#define COLUMN_MOIS   2
#define COLUMN_JOUR   3


char type_terrain[20];
int fekher;
int temps;
char* ID;
int check[]={0,0};


void
on_RBSOUSTERRAIN_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) 
{ 
strcpy(type_terrain, "Sous-terrain"); 
} 

}


void
on_RBSURFACE_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) 
{ 
strcpy(type_terrain, "Surface");
}
}


void
on_RBAERIEN_toggled                    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) 
{ 
strcpy(type_terrain, "Aérien");
}
}


void
on_confirmation_fekher_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
    {
	fekher=2;
    }
}


void
on_ajouter_parking_clicked             (GtkWidget       *objet,
                                        gpointer         user_data)
{
   GtkWidget *dialog;
    int v;
    GtkWidget *togglebutton1, *togglebutton2, *togglebutton3;

    GtkWidget *input_id;
    GtkWidget *input_nom;
    GtkWidget *input_region;
    GtkWidget *input_capacite;
    GtkWidget *input_nombre_bloc;
    GtkWidget *spin_jour;
    GtkWidget *spin_mois;
    GtkWidget *spin_annee;

    FILE *f = fopen("parkings.txt", "a+");

    if (f == NULL) {
        dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Erreur lors de l'ouverture du fichier.");
        gtk_window_set_title(GTK_WINDOW(dialog), "Erreur");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        Parking p;

        togglebutton1 = lookup_widget(objet, "RBSOUSTERRAIN");
        togglebutton2 = lookup_widget(objet, "RBSURFACE");
        togglebutton3 = lookup_widget(objet, "RBAERIEN");

        input_id = lookup_widget(objet, "EID");
        input_nom = lookup_widget(objet, "ENOM");
        input_region = lookup_widget(objet, "CBREGION_ajout");
        input_capacite = lookup_widget(objet, "ECAPACITE");
        input_nombre_bloc = lookup_widget(objet, "SBBLOC_ajout");
        spin_jour = lookup_widget(objet, "SBJJ_ajout");
        spin_mois = lookup_widget(objet, "SBMM_ajout");
        spin_annee = lookup_widget(objet, "SBYYYY_ajout");

        p.id_parking = atoi(gtk_entry_get_text(GTK_ENTRY(input_id)));
        strcpy(p.nom_parking, gtk_entry_get_text(GTK_ENTRY(input_nom)));
        strcpy(p.region_parking, gtk_combo_box_get_active_text(GTK_COMBO_BOX(input_region)));
        p.capacite = atoi(gtk_entry_get_text(GTK_ENTRY(input_capacite)));
        p.nombre_bloc = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input_nombre_bloc));
         
        // Gestion du type de terrain
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton1))) {
        strcpy(p.type_terrain, "Sous-terrain");
        } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton2))) {
        strcpy(p.type_terrain, "Surface");
        } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton3))) {
        strcpy(p.type_terrain, "Aérien");
        }
        p.nbPlaces=p.capacite;
        strcpy(p.id_agent,"null");
        // Construire la date à partir des spin buttons
        int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
        int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
        int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));
        sprintf(p.date_ajout, "%02d-%02d-%d", jour, mois, annee);

        // Vérifier les données avant d'ajouter
        if (p.id_parking == 0 || strlen(p.nom_parking) == 0 || strlen(p.region_parking) == 0) {
            dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_WARNING,
                                            GTK_BUTTONS_OK,
                                            "Données invalides. Veuillez remplir tous les champs.");
            gtk_window_set_title(GTK_WINDOW(dialog), "Avertissement");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        } else {
            // Ajouter le parking au fichier
            ajouterParking(p);
            fclose(f);

            dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_INFO,
                                            GTK_BUTTONS_OK,
                                            "Parking ajouté avec succès.");
            gtk_window_set_title(GTK_WINDOW(dialog), "Succès");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
}





void
on_annuler_clicked                     (GtkWidget       *objet,
                                        gpointer         user_data)
{
 GtkWidget *Ajout;
    GtkWidget *GestionParky;
    GtkWidget *treeview;

    // Lookup and destroy the current Ajout widget
    Ajout = lookup_widget(objet, "Ajout");
    gtk_widget_destroy(Ajout);

    // Create and show the GestionParky widget
    GestionParky = create_GestionParky();
    gtk_widget_show(GestionParky);

    // Retrieve the treeview and set up the row activation signal
    treeview = lookup_widget(GestionParky, "treeview_parking");
    
    // Cast treeview to GtkTreeView* and call afficherParking
    afficherParking((GtkTreeView *)treeview);
}


void
on_Ajouter_PARKY_clicked               (GtkWidget       *objet,
                                        gpointer         user_data)
{
GtkWidget *Ajout;
    GtkWidget *GestionParky;
   
    // Récupérer le widget de la fenêtre actuelle
    GestionParky = lookup_widget(objet, "GestionParky");
    gtk_widget_destroy(GestionParky);  // Fermer la fenêtre actuelle
   
    // Créer une nouvelle fenêtre pour l'ajout d'un parking
    Ajout = create_Ajout();  // Crée la fenêtre d'ajout de parking
    gtk_widget_show(Ajout);

}
void
on_modifier_fekher_clicked             (GtkWidget       *objet,
                                        gpointer         user_data)
{
     Parking p;

    // Récupérer les widgets
    GtkWidget *entry_recherche;
    entry_recherche = lookup_widget(objet, "ERECHERCHE");
// Récupérer l'ID saisi dans le champ de recherche
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
    int id = atoi(id_str);
    

    GtkWidget *input_nom = lookup_widget(objet, "ENOM_m");
    GtkWidget *input_region = lookup_widget(objet, "CBREGION_m");
    GtkWidget *input_capacite = lookup_widget(objet, "ECAPACITE_m");
    GtkWidget *input_nombre_bloc = lookup_widget(objet, "SBBLOC_m");
    GtkWidget *spin_jour = lookup_widget(objet, "SBJJ_m");
    GtkWidget *spin_mois = lookup_widget(objet, "SBMM_m");
    GtkWidget *spin_annee = lookup_widget(objet, "SBYYYY_m");

    GtkWidget *togglebutton1 = lookup_widget(objet, "RBSOUSTERRAIN_m");
    GtkWidget *togglebutton2 = lookup_widget(objet, "RBSURFACE_m");
    GtkWidget *togglebutton3 = lookup_widget(objet, "RBAERIEN_m");

    // Récupérer les valeurs des widgets
    p.id_parking = id;
    strcpy(p.nom_parking, gtk_entry_get_text(GTK_ENTRY(input_nom)));
    strcpy(p.region_parking, gtk_combo_box_get_active_text(GTK_COMBO_BOX(input_region)));
    p.capacite = atoi(gtk_entry_get_text(GTK_ENTRY(input_capacite)));
    p.nombre_bloc = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input_nombre_bloc));

    // Construire la date à partir des spin buttons
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));
    sprintf(p.date_ajout, "%02d-%02d-%d", jour, mois, annee);

    // Déterminer le type de terrain à partir de la variable globale
    // Assurez-vous que type_terrain est bien défini globalement
    strcpy(p.type_terrain, type_terrain);

    // Appeler la fonction pour modifier le parking
    modifierParking(p);

    // Afficher un message de confirmation
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(objet)),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Parking modifié avec succès.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}



void
on_afficher_fekher_clicked             (GtkWidget       *objet,
                                        gpointer         user_data)
{
GtkTreeView *treeview;
treeview=GTK_TREE_VIEW(lookup_widget(objet, "treeview_parking"));
afficherParking(treeview);
}



 
void
on_supprimer_fekher_clicked(GtkWidget *objet, gpointer user_data)
{

GtkTreeSelection *selection;
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkWidget *treeview;

    // Récupérer le TreeView
    treeview = lookup_widget(GTK_WIDGET(objet), "treeview_parking");
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    // Vérifier si une ligne est sélectionnée
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        int id_parking;

        // Récupérer l'ID de la ligne sélectionnée (colonne 0)
        gtk_tree_model_get(model, &iter, 0, &id_parking, -1);

        // Créer une boîte de dialogue pour confirmer la suppression
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_QUESTION,
                                                   GTK_BUTTONS_YES_NO,
                                                   "Voulez-vous vraiment supprimer le parking avec ID %d ?", id_parking);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response == GTK_RESPONSE_YES) {
            // Supprimer le parking du fichier
            supprimerParking(id_parking);

            // Supprimer la ligne du TreeView
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

            // Afficher un message de succès
            GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                               GTK_DIALOG_MODAL,
                                                               GTK_MESSAGE_INFO,
                                                               GTK_BUTTONS_OK,
                                                               "Le parking avec ID %d a été supprimé avec succès.", id_parking);
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
        }
    } else {
        // Aucune ligne sélectionnée
        GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                         GTK_DIALOG_MODAL,
                                                         GTK_MESSAGE_WARNING,
                                                         GTK_BUTTONS_OK,
                                                         "Veuillez sélectionner un parking à supprimer.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }
}



void
on_loop_clicked                        (GtkWidget       *objet,
                                        gpointer         user_data)
{
	const char *regions[] = 
	{
	    "Centre_ville_Tunis",
	    "Sidi_Bousaid",
	    "El_Manzeh",
	    "Lac1",
	    "Lac2",
	    "Marsa",
	    "Carthage",
	    "Centre_ville_Manouba",
	    "Zone_Indus_Manouba",
	    "Centre_Ville_Ben_Arous",
	    "Zone_Indus_Ben_Arous",
	    "Azur_City",
	    "Tunis_City",
	    "Manar_City",
	    "Tunisia_Mall"
	};
	const int NUM_REGIONS = sizeof(regions) / sizeof(regions[0]);

	GtkWidget *entry_recherche, *output_nom, *output_region, *output_capacite, *output_blocs;
	GtkWidget *output_sousterrain, *output_surface, *output_aerien;
	GtkWidget *output_annee, *output_mois, *output_jour;

    // Récupérer les widgets
    entry_recherche = lookup_widget(objet, "ERECHERCHE");
    output_nom = lookup_widget(objet, "ENOM_m");
    output_region = lookup_widget(objet, "CBREGION_m");
    output_capacite = lookup_widget(objet, "ECAPACITE_m");
    output_blocs = lookup_widget(objet, "SBBLOC_m");
    output_sousterrain = lookup_widget(objet, "RBSOUSTERRAIN_m");
    output_surface = lookup_widget(objet, "RBSURFACE_m");
    output_aerien = lookup_widget(objet, "RBAERIEN_m");
    output_annee = lookup_widget(objet, "SBYYYY_m");
    output_mois = lookup_widget(objet, "SBMM_m");
    output_jour = lookup_widget(objet, "SBJJ_m");

    // Récupérer l'ID saisi dans le champ de recherche
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
    int id = atoi(id_str);

    // Vérification de l'ID
    if (id <= 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(objet)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer un ID de parking valide.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return; // Sortir de la fonction si l'ID est invalide
    }

    // Recherche du parking
    Parking p = chercherParking(id);

    if (p.id_parking != 0) {
        // Remplir les widgets avec les données du parking
        gtk_entry_set_text(GTK_ENTRY(output_nom), p.nom_parking);

        // Mettre à jour la région
        int index = region_index(p.region_parking);
        if (index != -1) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(output_region), index);
        } else {
            gtk_combo_box_set_active(GTK_COMBO_BOX(output_region), -1);
        }

        // Convertir et afficher la capacité
        char capacite_str[16];
        snprintf(capacite_str, sizeof(capacite_str), "%d", p.capacite);
        gtk_entry_set_text(GTK_ENTRY(output_capacite), capacite_str);

        gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_blocs), p.nombre_bloc);

        // Type de terrain
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(output_sousterrain), strcmp(p.type_terrain, "Sous-terrain") == 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(output_surface), strcmp(p.type_terrain, "Surface") == 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(output_aerien), strcmp(p.type_terrain, "Aérien") == 0);

        // Date d'ajout
        int annee, mois, jour;
        sscanf(p.date_ajout, "%d-%d-%d", &annee, &mois, &jour);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_annee), annee);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_mois), mois);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_jour), jour);
    } else {
        // Réinitialisation des champs si le parking n'est pas trouvé
        gtk_entry_set_text(GTK_ENTRY(output_nom), "");
        gtk_combo_box_set_active(GTK_COMBO_BOX(output_region), -1);
        gtk_entry_set_text(GTK_ENTRY(output_capacite), "0");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_blocs), 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(output_sousterrain), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(output_surface), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(output_aerien), FALSE);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_annee), 2024);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_mois), 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_jour), 1);

        // Afficher un message d'erreur
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(objet)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Parking introuvable pour l'ID %d.", id);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}


void
on_RBSOUSTERRAIN_m_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton)) {
        strcpy(type_terrain, "Sous-terrain");
}
}

void
on_RBSURFACE_m_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton)) {
        strcpy(type_terrain, "Surface");
}
}


void
on_RBAERIEN_m_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton)) {
        strcpy(type_terrain, "Aérien");
}
}







/*
void
on_CBdate_affichage_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *jour = lookup_widget(GTK_WIDGET(togglebutton), "SBJJ_affichage");
    GtkWidget *mois = lookup_widget(GTK_WIDGET(togglebutton), "SBMM_affichage");
    GtkWidget *annee = lookup_widget(GTK_WIDGET(togglebutton), "SBYYYY_affichage");

    if (gtk_toggle_button_get_active(togglebutton)) {
        gtk_widget_set_sensitive(jour, TRUE);
        gtk_widget_set_sensitive(mois, TRUE);
        gtk_widget_set_sensitive(annee, TRUE);
    } else {
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(jour), 0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(mois), 0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(annee), 0);
        gtk_widget_set_sensitive(jour, FALSE);
        gtk_widget_set_sensitive(mois, FALSE);
        gtk_widget_set_sensitive(annee, FALSE);
    }
}


void
on_CBregion_affichage_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
GtkWidget *combobox_region = lookup_widget(GTK_WIDGET(togglebutton), "CBREGION_affichage");

    if (gtk_toggle_button_get_active(togglebutton)) {
        gtk_widget_set_sensitive(combobox_region, TRUE);
    } else {
        gtk_widget_set_sensitive(combobox_region, FALSE);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_region), -1);
    }
}
*/
/*
void
on_affecter_agent_clicked              (GtkWidget       *objet,
                                        gpointer         user_data)
{
GtkWidget *combobox_parking;
    GtkWidget *combobox_agent;
    int id_parking;
    char * id_agent;
    // Récupérer les widgets
    combobox_parking = GTK_WIDGET(gtk_builder_get_object(user_data, "combobox_parking"));
    combobox_agent = GTK_WIDGET(gtk_builder_get_object(user_data, "combobox_agent"));

    // Récupérer les IDs sélectionnés dans les combobox
    id_parking = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_parking));
    id_agent = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_agent));

    // Vérifier que des valeurs ont été sélectionnées
    if (id_parking == -1 || id_agent == -1) {
        g_print("Erreur : veuillez sélectionner un parking et un agent.\n");
        return;
    }

    // Appeler la fonction d'affectation
    //affecterParking(id_parking, id_agent);

    // Afficher un message de confirmation
    g_print("Affectation réussie : Parking ID %d -> Agent ID %s\n", id_parking, id_agent);
}*/



void
on_annuler_affectation_clicked         (GtkWidget       *objet ,
                                        gpointer         user_data)
{
GtkWidget *combobox_parking;
    GtkWidget *combobox_agent;

    // Récupérer les widgets
    combobox_parking = GTK_WIDGET(gtk_builder_get_object(user_data, "combobox_parking"));
    combobox_agent = GTK_WIDGET(gtk_builder_get_object(user_data, "combobox_agent"));

    // Réinitialiser les sélections
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_parking), -1);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_agent), -1);
}


void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *parking_name;

    // Obtenir le modèle du treeview
    model = gtk_tree_view_get_model(treeview);

    // Vérifier si la ligne est valide
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        // Récupérer la valeur de la colonne 1 (nom du parking)
        gtk_tree_model_get(model, &iter, 0, &parking_name, -1);

        // Afficher un message ou effectuer une action
        g_print("Parking sélectionné : %s\n", parking_name);

        // Libérer la mémoire
        g_free(parking_name);
    }
}

/*
void
on_affecter_fekher_clicked             (GtkButton       *objet,
                                        gpointer         user_data)
{
GtkWidget *affectation;
    GtkWidget *GestionParky;
   
    // Récupérer le widget de la fenêtre actuelle
    GestionParky = lookup_widget(objet, "GestionParky");
    gtk_widget_destroy(GestionParky);  // Fermer la fenêtre actuelle
   
    // Créer une nouvelle fenêtre pour l'affectation d'un parking
    affectation = create_affectation();  // Crée la fenêtre d'affectation de parking
    gtk_widget_show(affectation);
}
*/


void
 on_treeview_parking_row_activated(GtkTreeView       *treeview,
                                  GtkTreePath       *path,
                                  GtkTreeViewColumn *column,
                                  gpointer           user_data)
{
    GtkTreeIter iter;
    int id; // ID en tant qu'entier
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);

    // Vérification si l'itérateur est valide pour la ligne activée
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        // Récupérer l'ID du parking
        gtk_tree_model_get(model, &iter, 0, &id, -1);

        // Créer une boîte de dialogue pour confirmer la suppression
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_QUESTION,
                                                   GTK_BUTTONS_YES_NO,
                                                   "Voulez-vous vraiment supprimer le parking avec ID %d ?",
                                                   id);

        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response == GTK_RESPONSE_YES) {
            // Supprimer le parking
            supprimerParking(id);

            // Supprimer la ligne du TreeView
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

            // Afficher un message de succès
            GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                               GTK_DIALOG_MODAL,
                                                               GTK_MESSAGE_INFO,
                                                               GTK_BUTTONS_OK,
                                                               "Le parking avec ID %d a été supprimé avec succès.",
                                                               id);
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
        }
    } else {
        // Message d'erreur si l'itérateur est invalide
        GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                         GTK_DIALOG_MODAL,
                                                         GTK_MESSAGE_ERROR,
                                                         GTK_BUTTONS_OK,
                                                         "Impossible de récupérer les informations de la ligne activée.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }
}



void
on_return_affect_clicked               (GtkWidget       *objet,
                                        gpointer         user_data)
{
GtkWidget *GestionParky;
    GtkWidget *affectation;
   
    // Récupérer le widget de la fenêtre actuelle
    affectation = lookup_widget(objet, "affectation");
    gtk_widget_destroy(affectation);  // Fermer la fenêtre actuelle
   
    GestionParky = create_GestionParky(); 
    gtk_widget_show(GestionParky);
 }


void on_affectation_agent_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_id_agent;
    int id_parking_selected;
    char new_id_agent[20];

    // Récupérer l'Entry pour l'ID de l'agent
    entry_id_agent = lookup_widget(button, "entry_agent_affect");
    if (entry_id_agent == NULL) {
        g_print("Erreur : Widget 'entry_id_agent' introuvable dans l'interface.\n");
        return;
    }

    // Récupérer le TreeView depuis l'interface
    GtkWidget *treeview = lookup_widget(button, "treeview1");
    if (treeview == NULL) {
        g_print("Erreur : Le widget 'treeview1' est introuvable.\n");
        return;
    }

    // Récupérer l'ID du parking sélectionné
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter iter;

    // Vérifier si un élément est sélectionné
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_WARNING,
                                                  GTK_BUTTONS_OK,
                                                  "Aucun parking sélectionné. Veuillez sélectionner un parking.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Si un parking est sélectionné, récupérer l'ID du parking
    gtk_tree_model_get(model, &iter, 0, &id_parking_selected, -1); // Récupérer l'ID du parking (première colonne)

    // Vérifier que l'ID du parking est valide
    if (id_parking_selected == -1) {
        g_print("Erreur : L'ID du parking est invalide.\n");
        return;
    }

    // Récupérer l'ID de l'agent depuis le champ d'entrée
    char *id_agent_text = gtk_entry_get_text(GTK_ENTRY(entry_id_agent));
    if (strlen(id_agent_text) == 0) {
        g_print("Erreur : L'identifiant de l'agent ne peut pas être vide.\n");
        return;
    }

    // Copier l'ID de l'agent dans le tableau de caractères
    strncpy(new_id_agent, id_agent_text, sizeof(new_id_agent) - 1);
    new_id_agent[sizeof(new_id_agent) - 1] = '\0';

    // Chercher le parking avec l'ID sélectionné
    Parking parking = chercherParking(id_parking_selected);
    if (parking.id_parking == -1) {
        g_print("Erreur : Parking avec ID %d non trouvé.\n", id_parking_selected);
        return;
    }

    // Vérifier si l'ID de l'agent est "null" avant de mettre à jour
    if (strcmp(parking.id_agent, "null") != 0 && strlen(parking.id_agent) > 0) {
        g_print("Erreur : Le parking a déjà un agent affecté.\n");
        return;
    }

    // Mettre à jour l'ID de l'agent du parking trouvé
    strcpy(parking.id_agent, new_id_agent);

    // Modifier le parking dans le fichier
    if (modifier_Parking("parkings.txt", parking.id_parking, parking) == 1) {
        g_print("Modification réussie.\n");
    } else {
        g_print("Erreur lors de la modification du parking.\n");
        return;
    }

    // Mettre à jour l'affichage dans le TreeView
    afficherparking2(GTK_TREE_VIEW(treeview));

    // Optionnel : Afficher un message de confirmation
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                              GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_INFO,
                                              GTK_BUTTONS_OK,
                                              "L'agent a été affecté au parking avec succès.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}



void
on_afficher_affect_agent_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *affectation; // Fenêtre de l'interface "affectation"
    GtkWidget *treeview;           // Widget GtkTreeView où afficher les données

    // Charger l'interface "affectation" depuis GtkBuilder
    GtkBuilder *builder = GTK_BUILDER(user_data);

    // Récupérer la fenêtre "affectation"
    affectation = affectation = create_affectation ();
     
    if (affectation != NULL) {
        // Afficher la fenêtre "affectation" si elle est cachée
        gtk_widget_show (affectation);

        // Récupérer le GtkTreeView dans l'interface "affectation"
        treeview = lookup_widget(affectation, "treeview1");

        if (treeview != NULL) {
            // Appeler la fonction pour afficher la liste des parkings sans agent
            afficherparking2(GTK_TREE_VIEW(treeview));
        } else {
            g_print("Erreur : Le widget 'treeview1' est introuvable dans l'interface 'affectation'.\n");
        }
    } else {
        g_print("Erreur : L'interface 'affectation' est introuvable.\n");
    }
}


///////souleymene///////


void
on_btnAnnulerAdd_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
   GtkWidget *Ajouter_Reservation;
    GtkWidget *Liste_de_Reservation;
    
    GtkWidget *treeview;
   
    Ajouter_Reservation= lookup_widget(button, "Ajouter_Reservation");
    gtk_widget_destroy(Ajouter_Reservation);
    Liste_de_Reservation= lookup_widget(button, "Reservations_Listes");
    Liste_de_Reservation = create_Reservations_Listes();
    gtk_widget_show (Liste_de_Reservation);

    treeview= lookup_widget(Liste_de_Reservation, "tvReservation");

    // Ajouter le signal row_activated pour supprimer une réservation 
    g_signal_connect(treeview, "row_activated", G_CALLBACK(on_tvReservation_row_activated), NULL);
    afficher_reservation(treeview);
}



void on_btnAjouterRes_clicked(GtkButton *button, gpointer user_data) {
    // Déclaration des widgets
    GtkWidget *id, *jour, *mois, *annee, *heure, *minute, *duree;
    GtkWidget *labelerr, *treeview;
    Reservation r;
    Parking p;
    int out, id_park = -1;
    GtkTreeModel *model;
    GtkTreeIter iter;
    char parking_id[10];
    GtkWidget *dialog;

    // Récupération des widgets de l'interface
    id = lookup_widget(button, "entryIdRes");
    jour = lookup_widget(button, "spinJourRes");
    mois = lookup_widget(button, "spinMoisRes");
    annee = lookup_widget(button, "spinAnneeRes");
    heure = lookup_widget(button, "spinHeureRes");
    minute = lookup_widget(button, "spinMinuteRes");
    duree = lookup_widget(button, "spinDureeRes");
    labelerr = lookup_widget(button, "labelErreur");
    treeview = lookup_widget(button, "tvParkingA");

    // Vérification de la sélection dans le TreeView
    if (!gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)) ||
        !gtk_tree_selection_get_selected(GTK_TREE_SELECTION(gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview))), &model, &iter)) {
        // Afficher une boîte de dialogue si aucun parking n'est sélectionné
        dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Veuillez sélectionner un parking.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Récupération de l'ID du parking sélectionné
    gtk_tree_model_get(model, &iter, 0, &id_park, -1);  // Supposons que l'ID est dans la première colonne
    if (id_park == -1) {
        dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Veuillez sélectionner un parking valide.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Recherche du parking correspondant
    p = chercherParking("parkings.txt", id_park);
    if (p.id_parking == -1) {  // Vérification si le parking est trouvé
        gtk_label_set_text(GTK_LABEL(labelerr), "Parking introuvable.");
        return;
    }

    // Vérification des places disponibles
    if (p.nbPlaces <= 0) {
        gtk_label_set_text(GTK_LABEL(labelerr), "Le parking sélectionné est plein.");
        return;
    }

    // Mise à jour des places disponibles
    p.nbPlaces--;
    modifierParking("parkings.txt", p);

    // Affectation des données à la structure `Reservation`
    strcpy(r.id_reservation, gtk_entry_get_text(GTK_ENTRY(id)));

    // Date de début
    r.date_debut.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(jour));
    r.date_debut.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mois));
    r.date_debut.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(annee));

    // Heure et minute
    r.Hdebut = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(heure));
    r.Mdebut = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(minute));

    // Durée
    r.duree = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(duree));

    // Nature de la durée (à remplacer par une valeur définie)
    r.natureD = 1;  // Par exemple, 1 pour "heures"

    // Simulation d'utilisateur et parking (à adapter si nécessaire)
    strcpy(r.id_utilisateur, "nul");  // Exemple d'utilisateur statique
    sprintf(parking_id, "%d", id_park);
    strcpy(r.id_parking, parking_id);

    // Ajout de la réservation
    out = ajouterReservation("reservation.txt", r);

    // Affichage d'un message d'erreur ou de confirmation
    if (out == 0) {
        gtk_label_set_text(GTK_LABEL(labelerr), "Problème d'ajout de la réservation.");
    } else {
        gtk_label_set_text(GTK_LABEL(labelerr), "Réservation ajoutée avec succès !");
    }
}






void
on_btnAnnulerModRes_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *Modification;
    GtkWidget *Liste_de_Reservation;
    
    GtkWidget *treeview;
   
    Modification= lookup_widget(button, "Modification");
    gtk_widget_destroy(Modification);
    Liste_de_Reservation= lookup_widget(button, "Reservations_Listes");
    Liste_de_Reservation = create_Reservations_Listes();
    gtk_widget_show (Liste_de_Reservation);

    treeview= lookup_widget(Liste_de_Reservation, "tvReservation");

    // Ajouter le signal row_activated pour supprimer une réservation 
    g_signal_connect(treeview, "row_activated", G_CALLBACK(on_tvReservation_row_activated), NULL);
    afficher_reservation(treeview);
}

void
on_btnRechercher_clicked(GtkButton *button, gpointer user_data) {
    // Déclaration des widgets
    GtkWidget *id, *jour, *mois, *annee, *heure, *minute, *duree;
    GtkWidget *labelerr, *radioH, *radioJ;
    Reservation r;

    // Récupération des widgets de l'interface
    id = lookup_widget(button, "entryID_Res");
    jour = lookup_widget(button, "spinJourModRes");
    mois = lookup_widget(button, "spinMoisModRes");
    annee = lookup_widget(button, "spinAnneeModRes");
    heure = lookup_widget(button, "spinHeureModRes");
    minute = lookup_widget(button, "spinMinuteModRes");
    duree = lookup_widget(button, "spinDureeModRes");
    radioH = lookup_widget(button, "radioHeureModRes");
    radioJ = lookup_widget(button, "radioJourModRes");
    labelerr = lookup_widget(button, "labelErreurM");

    // Récupération de l'ID de réservation
   // strcpy(ID,gtk_entry_get_text(GTK_ENTRY(id)));
    ID = gtk_entry_get_text(GTK_ENTRY(id));

    // Vérification si l'entrée ID est vide
    if (ID == NULL || strcmp(ID, "") == 0) {
        gtk_label_set_text(GTK_LABEL(labelerr), "Veuillez entrer un identifiant.");
        return;
    }

    // Rechercher la réservation
    r = rechercherReservation("reservation.txt", ID);

    // Vérifier si la réservation existe
    if (strcmp(r.id_reservation, "") != 0) {
        // Mise à jour des champs avec les données de la réservation trouvée
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(jour), r.date_debut.jour);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(mois), r.date_debut.mois);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(annee), r.date_debut.annee);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(heure), r.Hdebut);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(minute), r.Mdebut);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(duree), r.duree);

        // Cocher le bon bouton radio pour la nature de la durée
        if (r.natureD == 0) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioH), TRUE);
        } else {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioJ), TRUE);
        }

        // Réinitialiser le label d'erreur
        gtk_label_set_text(GTK_LABEL(labelerr), "");
    } else {
        // Afficher un message d'erreur si la réservation n'existe pas
        gtk_label_set_text(GTK_LABEL(labelerr), "L'identifiant recherché n'existe pas.");
    }
}

void
on_btnModifierRes_clicked(GtkButton *button, gpointer user_data) {
    // Déclaration des widgets
    GtkWidget *id, *jour, *mois, *annee, *heure, *minute, *duree, *labelerr;
    Reservation r;
    int out;
    
    // Récupération des widgets de l'interface
    //id = lookup_widget(button, "entryID_Res");
    jour = lookup_widget(button, "spinJourModRes");
    mois = lookup_widget(button, "spinMoisModRes");
    annee = lookup_widget(button, "spinAnneeModRes");
    heure = lookup_widget(button, "spinHeureModRes");
    minute = lookup_widget(button, "spinMinuteModRes");
    duree = lookup_widget(button, "spinDureeModRes");
    labelerr = lookup_widget(button, "labelErreurM");

    // Vérification de l'ID de réservation
    //ID= gtk_entry_get_text(GTK_ENTRY(id));
    if (ID == NULL || strcmp(ID, "") == 0) {

        gtk_label_set_text(GTK_LABEL(labelerr), "Veuillez entrer un identifiant.");
        return;
    }
    strcpy(r.id_reservation, ID);

    // Affectation des champs de la structure `Reservation`
    r.date_debut.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(jour));
    r.date_debut.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mois));

    // Limitation de l'année entre 2024 et 2100
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(annee), 2024, 2100);
    r.date_debut.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(annee));

    r.Hdebut = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(heure));
    r.Mdebut = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(minute));
    r.duree = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(duree));

    // Nature de la durée (heures ou jours)
    r.natureD = temps;

    // Simulations d'ID utilisateur et parking (à remplacer par les données réelles)
    strcpy(r.id_utilisateur, "Nul");
    strcpy(r.id_parking, "Nul");
    r.etat=1;
    // Appel de la fonction `modifierReservation`
    out = modifierReservation("reservation.txt", ID, r);

    // Gestion des erreurs ou confirmation
    if (out == 0) {
        gtk_label_set_text(GTK_LABEL(labelerr), "Problème de modification de la réservation.");
    } else {
        gtk_label_set_text(GTK_LABEL(labelerr), "Réservation modifiée avec succès !");
    }
}
void
on_radioHeureModRes_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
        temps = 0; // Heure
    }
}
void
on_radioJourModRes_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
        temps = 1; // Jour
    }
}


void
on_btnAjouterFl_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *Ajouter_Reservation;
    GtkWidget *Liste_de_Reservation;
    
   
    Liste_de_Reservation= lookup_widget(button, "Reservations_Listes");
    gtk_widget_destroy(Liste_de_Reservation);
    
    Ajouter_Reservation = create_Ajouter_Reservation ();
    gtk_widget_show (Ajouter_Reservation);
}


void on_btnModifierFl_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer la fenêtre de la liste des réservations
    GtkWidget *window_liste = lookup_widget(GTK_WIDGET(button), "Reservations_Listes");

    // Récupérer la TreeView pour la liste des réservations
    GtkTreeView *treeview = GTK_TREE_VIEW(lookup_widget(window_liste, "tvReservation"));
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeModel *model;
    GtkTreeIter iter;

    // Vérifier si une ligne est sélectionnée
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *id_reservation;
        gtk_tree_model_get(model, &iter, 0,&id_reservation, -1);

        // Appeler la fonction de pré-remplissage avec l'ID de la réservation sélectionnée
        GtkWidget *Modification;
        Modification = create_Modification();

        // Recherchez les widgets nécessaires dans la fenêtre de modification
        GtkWidget *jour_spin = lookup_widget(Modification, "spinJourModRes");
        GtkWidget *mois_spin = lookup_widget(Modification, "spinMoisModRes");
        GtkWidget *annee_spin = lookup_widget(Modification, "spinAnneeModRes");
        GtkWidget *heure_spin = lookup_widget(Modification, "spinHeureModRes");
        GtkWidget *minute_spin = lookup_widget(Modification, "spinMinuteModRes");
        GtkWidget *duree_spin = lookup_widget(Modification, "spinDureeModRes");

        // Simulation de récupération des informations de la réservation à partir d'une source
        Reservation reservation;
        reservation = rechercherReservation("reservation.txt", id_reservation);

        if (strcmp(reservation.id_reservation, "") != 0) {
            ID = id_reservation;

            // Pré-remplir les champs
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(jour_spin), reservation.date_debut.jour);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(mois_spin), reservation.date_debut.mois);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(annee_spin), reservation.date_debut.annee);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(heure_spin), reservation.Hdebut);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(minute_spin), reservation.Mdebut);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(duree_spin), reservation.duree);

            // Cocher le bon bouton radio
            GtkWidget *radioH = lookup_widget(Modification, "radioHeureModRes");
            GtkWidget *radioJ = lookup_widget(Modification, "radioJourModRes");
            if (reservation.natureD == 0) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioH), TRUE);
            } else {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioJ), TRUE);
            }
        } else {
            g_warning("Réservation non trouvée pour l'ID : %s", id_reservation);
        }

        // Fermer la fenêtre de la liste des réservations
        gtk_widget_hide(window_liste);

        // Ouvrir la fenêtre de modification
        gtk_widget_show(Modification);
    } else {
        // Aucune ligne sélectionnée, afficher une boîte de dialogue
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window_liste),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez sélectionner une réservation avant de continuer."
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}




void
on_btnSupprimerFl_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkTreeView *treeview = GTK_TREE_VIEW(lookup_widget(GTK_WIDGET(button), "tvReservation"));
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *id_reservation;

        // Obtenir l'ID de la réservation sélectionnée
        gtk_tree_model_get(model, &iter, 0, &id_reservation, -1);

        // Demander confirmation avant suppression
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_QUESTION,
                                                   GTK_BUTTONS_YES_NO,
                                                   "Voulez-vous vraiment supprimer la réservation %s ?", id_reservation);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response == GTK_RESPONSE_YES) {
            // Supprimer la réservation du fichier
            supprimerReservation("reservation.txt",id_reservation);

            // Supprimer la ligne du TreeView
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

            // Message de succès
            GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                               GTK_DIALOG_MODAL,
                                                               GTK_MESSAGE_INFO,
                                                               GTK_BUTTONS_OK,
                                                               "La réservation %s a été supprimée avec succès.", id_reservation);
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
        }

        g_free(id_reservation);
    } else {
        // Aucune ligne sélectionnée
        GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                         GTK_DIALOG_MODAL,
                                                         GTK_MESSAGE_WARNING,
                                                         GTK_BUTTONS_OK,
                                                         "Veuillez sélectionner une réservation à supprimer.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }

}


void
on_btnAnnulerRes_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  // Récupérer la fenêtre de la liste des réservations
    GtkWidget *window_liste = lookup_widget(GTK_WIDGET(button), "Reservations_Listes");

    // Récupérer la TreeView pour la liste des réservations
    GtkTreeView *treeview = GTK_TREE_VIEW(lookup_widget(window_liste, "tvReservation"));
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeModel *model;
    GtkTreeIter iter;

    // Vérifier si une ligne est sélectionnée
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *id_reservation;
        gtk_tree_model_get(model, &iter, 0, &id_reservation, -1);

        // Confirmer l'annulation avec l'utilisateur
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window_liste),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO,
            "Êtes-vous sûr de vouloir annuler la réservation avec l'ID : %s ?", id_reservation
        );
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response == GTK_RESPONSE_YES) {
            // Appeler la fonction pour annuler la réservation
            int success = annulerReservation("reservation.txt", id_reservation);

            if (success) {
                // Afficher un message de succès
                GtkWidget *success_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(window_liste),
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_INFO,
                    GTK_BUTTONS_OK,
                    "La réservation a été annulée avec succès."
                );
                gtk_dialog_run(GTK_DIALOG(success_dialog));
                gtk_widget_destroy(success_dialog);

                // Mettre à jour l'affichage du TreeView (recharger les données)
                // Vous devez implémenter la fonction `charger_reservations_treeview`
                afficher_reservation(treeview);
            } else {
                // Afficher un message d'erreur
                GtkWidget *error_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(window_liste),
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Échec de l'annulation de la réservation. Veuillez réessayer."
                );
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            }
        }
    } else {
        // Aucune ligne sélectionnée, afficher un avertissement
        GtkWidget *warning_dialog = gtk_message_dialog_new(
            GTK_WINDOW(window_liste),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez sélectionner une réservation avant de continuer."
        );
        gtk_dialog_run(GTK_DIALOG(warning_dialog));
        gtk_widget_destroy(warning_dialog);
    }
}


void
on_btnTriRes_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
   GtkTreeView *treeview = GTK_TREE_VIEW(lookup_widget(button, "tvReservation"));
  GtkTreeSortable *sortable = GTK_TREE_SORTABLE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

    // Définir le tri décroissant sur la colonne des dates
    gtk_tree_sortable_set_sort_column_id(sortable, 4, GTK_SORT_DESCENDING);
}









void
on_radioHeureRes_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
        temps = 0; // Heure
    }
}


void
on_radioJourRes_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
 
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
        temps = 1; // Jours
    }

}

void
on_tvReservation_row_activated(GtkTreeView       *treeview,
                                GtkTreePath       *path,
                                GtkTreeViewColumn *column,
                                gpointer           user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *id_reservation;

    // Récupérer le modèle et l'itérateur pour la ligne activée
    model = gtk_tree_view_get_model(treeview);
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        // Obtenir l'ID de la réservation
        gtk_tree_model_get(model, &iter, 0, &id_reservation, -1);

        // Créer une boîte de dialogue pour confirmer la suppression
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_QUESTION,
                                        GTK_BUTTONS_YES_NO,
                                        "Voulez-vous vraiment supprimer la réservation %s ?", id_reservation);

        // Obtenir la réponse de l'utilisateur
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog); // Détruire la boîte de dialogue après usage

        // Traiter la réponse de l'utilisateur
        if (response == GTK_RESPONSE_YES) {
            // Supprimer la réservation du fichier
            supprimerReservation("reservation.txt", id_reservation);

            // Supprimer la ligne du TreeView
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

            // Afficher un message de succès
            GtkWidget *success_dialog;
            success_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview))),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "La réservation %s a été supprimée avec succès.", id_reservation);
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
        }

        // Libérer la mémoire allouée pour l'ID de réservation
        g_free(id_reservation);
    }
}




void
on_tvParkingA_row_activated            (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}
void
on_btnSearchPark_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *entryRegion = lookup_widget(button, "comboRegion");
    GtkTreeView *treeview = GTK_TREE_VIEW(lookup_widget(GTK_WIDGET(button), "tvParkingA"));
    char region[30];
    strcpy(region,gtk_combo_box_get_active_text(GTK_COMBO_BOX(entryRegion)));
    Parking *parkings = NULL;
    int taille = 0;

    // Rechercher les parkings par région
    parkings = rechercherParkingsParRegion("parkings.txt", region, &taille);

    // Trier les parkings trouvés par nombre de places
    if (taille > 0) {
        Parking *tri_parking = NULL;
        trierParkingParNbPlaces(parkings, &taille);
    }

    // Afficher les données triées dans le TreeView
    remplirTreeViewAvecDonnees(treeview, parkings, taille);

    if (parkings) free(parkings); // Libérer la mémoire
}

void
on_btnGestionReservation_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *Menu_User;
    GtkWidget *Liste_de_Reservation;
    GtkWidget *treeview;
   
    Menu_User= lookup_widget(button, "Menu_User");
    gtk_widget_destroy(Menu_User);
    Liste_de_Reservation= lookup_widget(button, "Reservations_Listes");
    Liste_de_Reservation = create_Reservations_Listes();
    gtk_widget_show (Liste_de_Reservation);

    treeview= lookup_widget(Liste_de_Reservation, "tvReservation");

    // Ajouter le signal row_activated pour supprimer une réservation 
    g_signal_connect(treeview, "row_activated", G_CALLBACK(on_tvReservation_row_activated), NULL);
    afficher_reservation(treeview);
}


void
on_btnGestionAvis_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btnAcceuilFl_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *Menu_User;
    GtkWidget *Reservations_Liste;
    
   
    Reservations_Liste= lookup_widget(button, "Reservations_Liste");
    
    gtk_widget_destroy(Reservations_Liste);
   Menu_User= lookup_widget(button, "Menu_User");
    Menu_User = create_Menu_User();
    gtk_widget_show (Menu_User);
   
}


void
on_btnAfficherFactureM_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  
}


void
on_checkStatus_toggled                 (GtkToggleButton *toggle_button,
                                        gpointer         user_data)
{
  GtkWidget *comboEtat = lookup_widget(GTK_WIDGET(toggle_button), "comboEtat");

    if (gtk_toggle_button_get_active(toggle_button)) {
        gtk_widget_set_sensitive(comboEtat, TRUE);
    } else {
        gtk_widget_set_sensitive(comboEtat, FALSE);
        gtk_combo_box_set_active(GTK_COMBO_BOX(comboEtat), -1); // Réinitialise
    }
}


void
on_checkDateDebut_toggled              (GtkToggleButton *toggle_button,
                                        gpointer         user_data)
{
   GtkWidget *jour = lookup_widget(GTK_WIDGET(toggle_button), "spinJourFl");
    GtkWidget *mois = lookup_widget(GTK_WIDGET(toggle_button), "spinMoisFl");
    GtkWidget *annee = lookup_widget(GTK_WIDGET(toggle_button), "spinAnneeFl");

    if (gtk_toggle_button_get_active(toggle_button)) {
        gtk_widget_set_sensitive(jour, TRUE);
        gtk_widget_set_sensitive(mois, TRUE);
        gtk_widget_set_sensitive(annee, TRUE);
    } else {
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(jour), 0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(mois), 0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(annee), 0);
        gtk_widget_set_sensitive(jour, FALSE);
        gtk_widget_set_sensitive(mois, FALSE);
        gtk_widget_set_sensitive(annee, FALSE);
    }
}


void on_btnFiltreRes_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview;
    GtkWidget *Liste_de_Reservation;
    Liste l = NULL;

    // Widgets des cases à cocher
    GtkWidget *checkDateDebut = lookup_widget(button, "checkDateDebut");
    GtkWidget *checkStatus = lookup_widget(button, "checkStatus");

    // Widgets des SpinButtons et ComboBox
    GtkWidget *jour = lookup_widget(button, "spinJourFl");
    GtkWidget *mois = lookup_widget(button, "spinMoisFl");
    GtkWidget *annee = lookup_widget(button, "spinAnneeFl");
    GtkWidget *comboEtat = lookup_widget(button, "comboEtat");

    // Récupérer les filtres activés
    gboolean filtreDate = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkDateDebut));
    gboolean filtreEtat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkStatus));

    // Récupération des valeurs des filtres
    int etat = -1; // Par défaut : pas de filtre par état
    if (filtreEtat) {
        const char *etat_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboEtat));
        if (etat_text != NULL) {
            if (strcmp(etat_text, "Annulee") == 0) {
                etat = 0;
            } else if (strcmp(etat_text, "Validee") == 0) {
                etat = 1;
            }
        }
    }

    Date dateDebut = {0, 0, 0}; // Par défaut : pas de filtre par date
    if (filtreDate) {
        dateDebut.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(jour));
        dateDebut.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mois));
        dateDebut.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(annee));
    }

    // Récupération du TreeView
    Liste_de_Reservation = lookup_widget(button, "Reservations_Listes");
    treeview = lookup_widget(Liste_de_Reservation, "tvReservation");

    // Récupération de l'ID utilisateur courant (exemple, à adapter)
    char *idU = "nul"; // ID par défaut

    // Appliquer le filtre
    l = filtreReservationCombine("reservation.txt", etat, dateDebut, idU);

    // Charger les résultats dans le TreeView
    if (l != NULL) {
        chargerListeFiltreeDansTreeView(treeview, l);
    } else {
        printf("Aucune réservation trouvée pour les filtres appliqués.\n");
    }
}




void
on_btnGestionAgent_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btnGestionCompte_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btnGestionService_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btnParking_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{

}

