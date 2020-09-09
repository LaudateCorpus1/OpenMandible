## OpenMandible source code and its usage

Both standalone and source code of the OpenMandible are provided, with two use-cases showing how to generate the whole masticatory system and its cut-part (Figure 5). The software was developed by using the Linux g++ compiler and the Code::Blocks IDE. Since no third-party librar-ies were used, the framework could be compiled for any platform (C++ makefiles are provided for both Windows and Linux OS). The C++ project includes the dfemtoolz library and its three mod-ules (remesh, openR, multi-material) - which are described in our previous study: *D.Z. Milasinovic, A.M. Vukicevic, N.D. Filipovic, dfemtoolz: An open-source C++ framework for efficient imposition of material and boundary conditions in finite element biomedical simulations, Computer Phys-ics Communications, 249 (2020) 106996, doi: 10.1016/j.cpc.2019.106996.* 

Generation of new simulation models starts with the definition of dynamic template arrays (Collection-s and SuperCollection-s classes from the dfemtoolz library), which stores all the model data and features:

    SuperCollection <Mesh_Node> nodez;
    SuperCollection <Geom_Element> elements;
    Collection <Geom_Element> contact_faces, surface_faces;

Workflow, and key data structures, of the framework will be explained following the analogy of the config file sections (which are described in Section 2.4):

**Step 1**: TetGen (or other meshing software) outputs are used to create tet4 or tet8 mesh of the mod-el. Briefly, the dfemtoolz_remesh module process each material provided as the Section A inputs:

    dremesh_get_elemsF(
    params->submodule,// this is user selection for tet4 or hex8 from .cfg file
    nodez[i], // Nodes of i-th material
    elements[i], // Elements of i-th material
    "input/materials/tetgen-files/" + params->materials_names[i] + ".1.node",
    "input/materials/tetgen-files/" + params->materials_names[i] + ".1.ele",
     "");

**Step 2**: Meshes of the constitutive materials are merged into the single multi-material model by us-ing the dfemtoolz_multimaterial module:
   
    merg_multimaterial(
    params, // singleton object that contains all the programme parameters
    nodez, // supercollection of nodes
    Elements, // supercollection of elements
    surface_faces, // collection of surface faces
    contact_faces, // collection of contact faces
    all_contacts); // supercollection of all contact faces

**Step 3**: Application of BCs from the input .stl files (from input/bc) was done by calling the dfemtoolz_openR module for each input command written into the C section of the config file:

    bc_openR(
    params, // singleton object that contains all the programme parameters
    nodez[1], // first collection of nodes in supercollection
    elements[1], // first collection of elements in supercollection
    surface_faces, // collection of surface faces
    contact_faces); // collection of contact faces


**Step 4**: Application of the directed BCs, which is similar to the step 3, with the difference that it uses two .stl files as inputs (the first .stl defines the insertion nodes, and the latter determines the origin of the directed BCs - in our case muscles).
    
    bc_directed(params, nodez[1], elements[1]);

**Step 5** - Software reads the osteons (input/aOsteonsFile.txt) pre-computed for the simplified man-dible model, and uses its data to 1) split the selected material (by default CorticalBone) into ana-tomical zones, and 2) prescribe orthotropy axes for each element of the selected material:

    material_ID_max = orthotropy(params, nodez[1], elements[1]);

All outputs are printed using the objects of singleton classes POS_Printer, VTK_Printer, and a set of functions used for plain text printing (listed in “case5-mandible_plain_text_print_functions.h”). Each function name is self-descriptive, so for example:

    if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
     print_plain_text_BC_nodes_file(
     params->prescribed_surfaces_filenames[i].name, 
     params->prescribed_surfaces_filenames[i].material, 
     normal_vector, 
     BCnodes, 
     "output/bc/BC_" + utos(i) + "_nodes_" + 
     params->prescribed_surfaces_filenames[i].name + "-tet.txt");
    // this call will print plain text BC nodes file if the elements are tetrahedrons (tet4)
    if (elements[1][1].how_many_nodes_per_element() == constants::BRICK)
    vtk_printer->print_quads_to_vtk_file(
     BC, nodez[1], 
     "output/bc/BC_" + utos(i) + "_faces_" + params->prescribed_surfaces_filenames[i].name + "-hex.vtk");
    // this call will print quadrilateral elements / faces to the appro-priate vtk file
