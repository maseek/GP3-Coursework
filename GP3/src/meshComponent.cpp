#include "pch.h"
#include "meshComponent.h"
#include "meshModel.h"
#include "game.h"

MeshComponent::MeshComponent( MeshModel& model ) : RenderComponent( ComponentType::Mesh ), _model( model )
{

}
MeshComponent::~MeshComponent( )
{
	
}
void MeshComponent::vRender( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld )
{
	_model.vRender( scene, time, camera, toWorld );
}
void MeshComponent::vInit( Game& game )
{
	// create a new scene for this and add it to the game
	std::shared_ptr< SceneNode > sceneNode( new SceneNode( this ) );
	game.addSceneNode( sceneNode, RenderPassType::GameObject );
	// set owners bounding sphere radius with the value from its mesh
	_owner->setRadius( _model.getRadius( _owner->getScale( ) ) );
}