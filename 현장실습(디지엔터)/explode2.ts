import CMovement from "../../lib/component/CMovement.js";
import CVec3 from "../../lib/geometry/CVec3.js";
import CObjBD from "../../lib/component/CObjBD.js";
import { CPaint3D } from "../../lib/component/CPaint.js";
import CVec4 from "../../lib/geometry/CVec4.js";
import CAnimation, { CClipCObjHD, CClipPRS, CClipColor, CClipPhysics } from "../../lib/component/CAnimation.js";
import CAniFlow from "../../lib/component/CAniFlow.js";
import CParticle, { CPTCComponent, CPTCShapeOut } from "../../lib/component/CPaticle.js";
import CFramework from "../../lib/util/CFramework.js";
import CMath from "../../lib/geometry/CMath.js";
import CWatch from "../../lib/basic/CWatch.js";
import CRenderPass from "../../lib/render/CRenderPass.js";
import Df from "../../lib/basic/Df.js";
import { CPTCFirework } from "./Firework.js";
import CRigidBody from "../../lib/component/CRigidBody.js";

export default function explode2(_can, _pos) {
	let center = new CObjBD();
	_pos.toCopy(center.GetPos());
	center.SetSca(new CVec3(0.1, 0.1, 0.1));
	
	let ani = new CAnimation();
	ani.Push(new CClipCObjHD(2000, true));
	center.PushCComponent(new CAniFlow(ani));
	
	MushroomCloud(_can, center, 600, 150, 2000);
	MushroomCloud(_can, center, 400, 50, 2000);
	MushroomCloud(_can, center, 200, 15, 2000);
	UpStreamCloud(_can, center, 600, 2000);
	ExplodeCircle(_can, center, new CVec3(0, 1, 0), 3000, 1000, 40);
	Debris2(_can, center, 4);
	
	_can.Push(center);
}

function LookRotation(_forward, _up) {
	_forward = CMath.V3Nor(_forward);
	
	let vec = CMath.V3Nor(_forward);
	let vec2 = CMath.V3Nor(CMath.V3Cross(_up, vec));
	let vec3 = CMath.V3Cross(vec, vec2);
	
	let m00 = vec2.x;
	let m01 = vec2.y;
	let m02 = vec2.z;
	let m10 = vec3.x;
	let m11 = vec3.y;
	let m12 = vec3.z;
	let m20 = vec.x;
	let m21 = vec.y;
	let m22 = vec.z;
	
	let num8 = (m00 + m11) + m22;
	let qut = new CVec4();
	if(num8 > 0) {
		let num = Math.sqrt(num8 + 1);
		qut.w = num * 0.5;
		num = 0.5 / num;
		qut.x = (m12 - m21) * num;
		qut.y = (m20 - m02) * num;
		qut.z = (m01 - m10) * num;
		return qut;
	}
	if((m00 >= m11) && (m00 >= m22)) {
		let num7 = Math.sqrt(((1 + m00) - m11) - m22);
		let num4 = 0.5 / num7;
		qut.x = 0.5 / num7;
		qut.y = (m01 + m10) * num4;
		qut.z = (m02 + m20) * num4;
		qut.w = (m12 - m21) * num4;
		return qut;
	}
	if(m11 > m22) {
		let num6 = Math.sqrt(((1 + m11) - m00) - m22);
		let num3 = 0.5 / num6;
		qut.x = (m10 + m01) * num3;
		qut.y = 0.5 * num6;
		qut.z = (m21 + m12) * num3;
		qut.w = (m20 - m02) * num3;
		return qut;
	}
	let num5 = Math.sqrt(((1 + m22) - m00) - m11);
	let num2 = 0.5 / num5;
	qut.x = (m20 + m02) * num2;
	qut.y = (m21 + m12) * num2;
	qut.z = 0.5 * num5;
	qut.w = (m01 - m10) * num2;
	return qut;
}

//axis need to be normailzed first
function RotateAroundAxis(vec, axis, radian) {
	let cross = CMath.V3Cross(axis, vec);
	let f = CMath.V3MulFloat(axis, CMath.V3Dot(axis, vec));
	let s = CMath.V3MulFloat(CMath.V3Cross(cross, axis), Math.cos(radian));
	let t = CMath.V3MulFloat(cross, Math.sin(radian));
	
	return CMath.V3AddV3(CMath.V3AddV3(f, s), t);
}

class CPTCMushroomCloud extends CWatch {
	m_posMin;
	m_posMax;
	m_colorMin;
	m_colorMax;
	m_speedMin;
	m_speedMax;
	m_lifeMin;
	m_lifeMax;
	m_dirMin;
	m_dirMax;
	m_scaleMin;
	m_scaleMax;
	
	constructor() {
		super();
		
	}
	
	Process(_obj, _rb) {
		if (this.m_dirMax.equals(this.m_dirMin) == false) {
            var dirX = Math.random() * (this.m_dirMax.x - this.m_dirMin.x) + this.m_dirMin.x;
            var dirY = Math.random() * (this.m_dirMax.y - this.m_dirMin.y) + this.m_dirMin.y;
            var dirZ = Math.random() * (this.m_dirMax.z - this.m_dirMin.z) + this.m_dirMin.z;
            var speed = Math.random() * (this.m_speedMax - this.m_speedMin) + this.m_speedMin;
            let mov = new CMovement("paticle", CMath.V3Nor(new CVec3(dirX, dirY, dirZ)), speed);
            mov.m_acc = -speed / (this.m_lifeMin / 1000);
            _rb.Push(mov);
            _rb.Push(new CMovement("invGravity", new CVec3(0, 1, 0), 100));
            _obj.SetPos(CMath.V3MulFloat(new CVec3(dirX, dirY, dirZ), 10));
        }
        
        let ani = new CAnimation();
        let life = Math.random() * (this.m_lifeMax - this.m_lifeMin) + this.m_lifeMin + 200;
        ani.Push(new CClipCObjHD(life, true));
        let clipColor = new CClipColor(0, life, this.m_colorMin, this.m_colorMax);
        clipColor.m_space = 2;
        ani.Push(clipColor);
        let scale = Math.random() * (this.m_scaleMax - this.m_scaleMin) + this.m_scaleMin;
        ani.Push(new CClipPRS(0, life - 200, CMath.V3MulFloat(new CVec3(1,1,1), scale / 2), CMath.V3MulFloat(new CVec3(1,1,1), scale),2));
        ani.Push(new CClipPRS(life - 200, 200, CMath.V3MulFloat(new CVec3(1,1,1), scale), new CVec3(0,0,0), 2));
        
        _obj.PushCComponent(new CAniFlow(ani));
	}
}

function MushroomCloud(_can, _obj, _radius, _num, _life) {
	let mushroomCloud = new CParticle();
	let ptcp = new CPTCComponent();
	let pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	let rp = new CRenderPass();
	rp.m_depthTest = true;
	rp.m_vf = "PreSimple";
	pt.SetRenderPass(rp);
	ptcp.m_comArr.push(pt);
	mushroomCloud.m_cptcArr.push(ptcp);
	
	let shape = new CPTCMushroomCloud();
	shape.m_posMin = new CVec3(-200, 0, -200);
	shape.m_posMax = new CVec3(200, 100, 200);
	shape.m_scaleMin = 0.1;
	shape.m_scaleMax = 0.3;
	shape.m_speedMin = _radius;
	shape.m_speedMax = _radius;
	shape.m_dirMin = new CVec3(-1,0,-1);
	shape.m_dirMax = new CVec3(1, 1, 1);
	shape.m_lifeMin = _life / 2;
	shape.m_lifeMax = _life;
	shape.m_colorMin = new CVec4(0, 1, 1, 0);
	shape.m_colorMax = new CVec4(0, 1, 0, 0);
	
	mushroomCloud.m_shape = shape;
	mushroomCloud.m_tartget = _obj;
	mushroomCloud.m_createCount = _num;
	mushroomCloud.m_createTime = 50;
	mushroomCloud.m_endTime = 50;
	
	_can.Push(mushroomCloud);
}

function UpStreamCloud(_can, _obj, _radius, _life) {
	let mushroomCloud = new CParticle();
	let ptcp = new CPTCComponent();
	let pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	let rp = new CRenderPass();
	rp.m_depthTest = true;
	rp.m_vf = "PreSimple";
	pt.SetRenderPass(rp);
	ptcp.m_comArr.push(pt);
	mushroomCloud.m_cptcArr.push(ptcp);
	
	let ani = new CAnimation();
	let aniTime = _life / 4 - 200;
	aniTime = aniTime > 0? aniTime : 100;
	ani.Push(new CClipPRS(0, aniTime * 0.8, new CVec3(0.2,0.2,0.2), new CVec3(0.2,0.2,0.2),2));
	ani.Push(new CClipPRS(aniTime * 0.8, aniTime * 0.2, new CVec3(0.2,0.2,0.2), new CVec3(0,0,0),2));
	let clipColor = new CClipColor(0,aniTime,new CVec4(0, 1, 1, 0), new CVec4(0, 1, 0, 0));
	clipColor.m_space = 2;
	ani.Push(clipColor);
	ani.Push(new CClipCObjHD(aniTime, true));
	ptcp.m_comArr.push(new CAniFlow(ani));
	
	let shape = new CPTCShapeOut();
	shape.m_speedMin = _radius;
	shape.m_speedMax = _radius;
	shape.m_dirMin = new CVec3(-0.5, 1, -0.5);
	shape.m_dirMax = new CVec3(0.5, 1, 0.5);
	
	mushroomCloud.m_shape = shape;
	mushroomCloud.m_tartget = _obj;
	mushroomCloud.m_createCount = 1;
	mushroomCloud.m_createTime = 100;
	mushroomCloud.m_endTime = _life / 3;
	
	_can.Push(mushroomCloud);
}

class CPTCExplodeCircle extends CPTCFirework {
	m_axis;
	m_lifeMin;
	m_lifeMax;
	m_angleMin;
	m_angleMax;
	m_colorMin;
	m_colorMax;
	m_scaleMax;
	m_scaleMin;
	m_count;
	m_speedMax;
	m_speedMin;
	
	m_indexRot;
	constructor() {
        super();
        this.m_axis = new CVec3(0, 1, 0);
        this.m_lifeMin = 500;
        this.m_lifeMax = 1000;
        this.m_angleMin = 0;
		this.m_angleMax = 60;
        this.m_indexRot = 0;
        this.m_count = 100;
        this.m_scaleMax = 0.3;
		this.m_scaleMin = 0.1;
        this.m_colorMax = new CVec4(1,1,1,0);
        this.m_colorMin = new CVec4(0,0,0,0);
        this.m_speedMax = 0;
        this.m_speedMin = 0;
    }
    Process(_obj, _rb) {
		this.m_indexRot += Math.PI * 2 / this.m_count;
		this.m_axis = CMath.V3Nor(this.m_axis);
		let arbitraryVec = new CVec3(1, 0, 0);
		arbitraryVec = (arbitraryVec == this.m_axis)? new CVec3(0,1,0) : arbitraryVec;
		let randomForward = CMath.V3Cross(this.m_axis, arbitraryVec);
		let dir = RotateAroundAxis(randomForward, this.m_axis, this.m_indexRot);
		dir = CMath.V3Nor(dir);
        var speed = Math.random() * (this.m_speedMax - this.m_speedMin) + this.m_speedMin;
        let movement = new CMovement("paticle", dir, speed);
        movement.m_acc = Math.random() * (this.m_accMax - this.m_accMin) + this.m_accMin;
        movement.m_minPow = 10;
        _rb.Push(movement);
        
        var posX = Math.random() * (this.m_posMax.x - this.m_posMin.x) + this.m_posMin.x;
        var posY = Math.random() * (this.m_posMax.y - this.m_posMin.y) + this.m_posMin.y;
        var posZ = Math.random() * (this.m_posMax.z - this.m_posMin.z) + this.m_posMin.z;
        _obj.SetPos(new CVec3(posX, posY, posZ));
        
        let ani = new CAnimation();
        let life = Math.random() * (this.m_lifeMax - this.m_lifeMin) + this.m_lifeMin;
        ani.Push(new CClipCObjHD(life, true));
        let clipPhy = new CClipPhysics(0, life);
		clipPhy.m_rotUse = true;
		let rotX = Math.random() * (this.m_angleMax - this.m_angleMin) + this.m_angleMin;
		let rotY = Math.random() * (this.m_angleMax - this.m_angleMin) + this.m_angleMin;
		let rotZ = Math.random() * (this.m_angleMax - this.m_angleMin) + this.m_angleMin;
		clipPhy.m_rot = CMath.EulerToQut(new CVec3(rotX, rotY, rotZ));
		ani.Push(clipPhy);
		
		let colorBefore = CMath.V4MulFloat(CMath.V4SubV4(this.m_colorMax, this.m_colorMin), Math.random() * 0.5);
		colorBefore = CMath.V4AddV4(colorBefore, this.m_colorMin);
		let colorAfter = CMath.V4MulFloat(CMath.V4SubV4(this.m_colorMax, this.m_colorMin), (1 + Math.random()) / 2);
		colorAfter = CMath.V4AddV4(colorAfter, this.m_colorMin);
		let clipColor = new CClipColor(0, life, colorBefore, colorAfter);
		clipColor.m_space = 2;
		clipColor.m_curve.m_type = 0;
		ani.Push(clipColor);
		
		let scale = Math.random() * (this.m_scaleMax - this.m_scaleMin) + this.m_scaleMin;
		let clipScale = new CClipPRS(0, life, CMath.V3MulFloat(new CVec3(1.35,1.35, 1.35), scale), new CVec3(), 2);
		clipScale.m_curve.m_type = 3;
		ani.Push(clipScale);
        
        _obj.PushCComponent(new CAniFlow(ani));
    }
}

function ExplodeCircle(_can, _obj, _axis, _radius, _life, _num) {
	let Particle = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = true;
	rp.m_depthWrite = false;
	rp.m_priority = Df.RenderPriority.Normal + 20;
	rp.m_vf = "PreSimple";
	let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	ptt.SetRenderPass(rp);
	ptcp.m_comArr.push(ptt);
	ptcp.m_rate = 4;
	
	let lifeTime = _life;
	
	let rb = new CRigidBody();
	rb.Push(new CMovement("gravity", new CVec3(0, -1, 0), 9.8));
	ptcp.m_comArr.push(rb);
	
	Particle.m_cptcArr.push(ptcp);
	
	let shape = new CPTCExplodeCircle();
	shape.m_accMin = -(_radius + 100) * 3;
	shape.m_accMax = -(_radius + 100) * 3;
	shape.m_speedMax = _radius + 100;
	shape.m_speedMin = _radius + 60;
	shape.m_lifeMax = lifeTime * 1.5;
	shape.m_lifeMin = lifeTime * 0.5;
	shape.m_angleMin = Math.PI * 0;
	shape.m_angleMax = Math.PI;
	shape.m_colorMin = new CVec4(0, 1, 0.3, 0);
	shape.m_colorMax = new CVec4(0,1,0,0);
	shape.m_scaleMax = 0.4;
	shape.m_scaleMin = 0.2;
	shape.m_axis = _axis;
	shape.m_count = _num;
	
	Particle.m_shape = shape;
	Particle.m_createCount = _num;
	Particle.m_createTime = 100;
	Particle.m_endTime = 100;
	Particle.m_target = _obj;
	
	_can.Push(Particle);
}

export class CPTCDebrisEffect extends CWatch {	
	m_life;
	m_dirMin;
	m_dirMax;
	m_speedMin;
	m_speedMax;
	m_endTime;
	m_particleColor;
	m_scaMax;
	m_scaMin;
	m_time;
	m_particleDisappearTime;
	m_canvas;
	
    constructor() {
        super();
        this.m_life = 1000; // 파편의 유지시간
        this.m_dirMin = new CVec3(-1, 0, 0);
        this.m_dirMax = new CVec3(1, 1, 0);
        this.m_speedMin = 100; //속도
        this.m_speedMax = 100; //속도
        this.m_endTime = 100; //몇 초에 한번씩 생성하는지
        this.m_particleColor = new CVec4(0,0,0,0);
        this.m_scaMax = 0.3;
        this.m_scaMin = 0.1;
        this.m_time = 300; //잔상의 유지시간
        this.m_particleDisappearTime = 100; //오브젝트가 사라진 후 잔상이 사라질 때 까지의 시간
        this.m_canvas = null;
    }
    Process(_obj, _rb) {
        if (this.m_dirMax.equals(this.m_dirMin) == false) {
            var dirX = Math.random() * (this.m_dirMax.x - this.m_dirMin.x) + this.m_dirMin.x;
            var dirY = Math.random() * (this.m_dirMax.y - this.m_dirMin.y) + this.m_dirMin.y;
            var dirZ = Math.random() * (this.m_dirMax.z - this.m_dirMin.z) + this.m_dirMin.z;
            var speed = Math.random() * (this.m_speedMax - this.m_speedMin) + this.m_speedMin;
            let norDir = CMath.V3Nor(new CVec3(dirX, dirY, dirZ));
            let move = new CMovement("paticle", norDir, speed);
            move.m_acc = -speed / (this.m_life / 1000);
            _rb.Push(move);
            
            //gravity
            let gr = new CMovement("gravity", new CVec3(0, -1, 0), 0);
            gr.m_minPow = -100;
            gr.m_acc = 100;
            _rb.Push(gr);
            
            let rot = new CVec3(Math.atan(dirZ / dirX), Math.atan(dirY / dirX), Math.atan(dirZ / dirY));
            _obj.SetRot(rot);
        }
        
        //light
        let light = new CObjBD();
        light.SetSca(CMath.V3MulFloat(_obj.GetSca(), 1.5));
        let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
        ptt.SetRGBA(new CVec4(0.4,0.4,0.4,1));
        let rpp = new CRenderPass();
        rpp.m_depthTest = true;
        rpp.m_depthWrite = false;
        rpp.m_priority = Df.RenderPriority.Normal + 1;
        rpp.m_vf = "PreSimple";
        ptt.SetRenderPass(rpp);
        light.PushCComponent(ptt);
        _obj.PushChilde(light);
        
        //ptc
        let smoke = new CParticle();
        let ptcp = new CPTCComponent();
        let rp = new CRenderPass();
        rp.m_depthTest = true;
        rp.m_vf = "PreSimple";
        
        let pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
        pt.SetRenderPass(rp);
        pt.SetRGBA(new CVec4(1,1,1,0));
        ptcp.m_comArr.push(pt);
        smoke.m_cptcArr.push(ptcp);
        
        let ani = new CAnimation();
        let lifeTime = 1000;
        ani.Push(new CClipCObjHD(lifeTime, true));
        let randomScale = (Math.random() + 1) / 20;
		ani.Push(new CClipPRS(0, 0, new CVec3(), CMath.V3MulFloat(new CVec3(1,1,1), randomScale), 2));
        ani.Push(new CClipPRS(lifeTime - 200, 200, CMath.V3MulFloat(new CVec3(1,1,1), randomScale), new CVec3(0,0,0), 2));
    	let clipColor = new CClipColor(0, lifeTime, new CVec4(0,1,1,0), new CVec4(0,1,0,0));
    	clipColor.m_space = 2;
    	ani.Push(clipColor);
    	ptcp.m_comArr.push(new CAniFlow(ani));
    	
    	let shape = new CPTCShapeOut();
    	shape.m_posMin = new CVec3(-10, -10, -10);
    	shape.m_posMin = new CVec3(10, 10, 10);
    	shape.m_speedMin = 0;
    	shape.m_speedMax = 0;
    	
    	smoke.m_shape = shape;
    	smoke.m_target = _obj;
    	smoke.m_createTime = 50;
    	smoke.m_createCount = 1;
    	smoke.m_endTime = this.m_life * 1.5;
    	
    	this.m_canvas.Push(smoke);
    }
}

function Debris2(_can, _center, _num) {
	//잔해
	let debris = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = true;
	rp.m_depthWrite = false;
	rp.m_priority = Df.RenderPriority.Normal + 2;
	rp.m_vf = "PreSimple";
	
	let pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	pt.SetRenderPass(rp);
	pt.SetRGBA(new CVec4(1,1,1,0));
	ptcp.m_comArr.push(pt);
	debris.m_cptcArr.push(ptcp);
	
	let ani = new CAnimation();
	let lifeTime = 3000;
	let lastTimeForTrailDisappear = 200;
	ani.Push(new CClipCObjHD(lifeTime + lastTimeForTrailDisappear, true));
	let scaMax = 0.1, scaMin = 0.05;
	let scaRan = CMath.V3MulFloat(new CVec3(1, 1, 1), Math.random() * (scaMax - scaMin) + scaMin);
	let clipPRS = new CClipPRS(0, lifeTime, scaRan, new CVec3(0, 0, 0), 2);
	clipPRS.m_curve.m_type = 2;
	ani.Push(clipPRS);
	let clipColor = new CClipColor(0, lifeTime, new CVec4(1,1,1,0), new CVec4(1,1,1,1));
	clipColor.m_curve.m_type = 2;
	ani.Push(clipColor);
	ptcp.m_comArr.push(new CAniFlow(ani));
	
	let shape2 = new CPTCDebrisEffect();
	shape2.m_dirMax = new CVec3(1, 1, 1);
	shape2.m_dirMin = new CVec3(-1, 1, -1);
	shape2.m_speedMin = 250;
	shape2.m_speedMax = 750;
	shape2.m_life = lifeTime + lastTimeForTrailDisappear;
	shape2.m_particleColor = new CVec4(1,1,1,0);
	shape2.m_particleDisappearTime = lastTimeForTrailDisappear;
	shape2.m_canvas = _can;
	debris.m_shape = shape2;
	
	debris.m_target = _center;
	debris.m_createTime = 100;
	debris.m_createCount = _num;
	debris.m_endTime = 100;
	
	_can.Push(debris);
}