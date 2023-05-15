import CMovement from "../../lib/component/CMovement.js";
import CVec3 from "../../lib/geometry/CVec3.js";
import CObjBD from "../../lib/component/CObjBD.js";
import { CPaint3D, CPaintBillbord } from "../../lib/component/CPaint.js";
import CVec4 from "../../lib/geometry/CVec4.js";
import CAnimation, { CClipCObjHD, CClipPRS, CClipColor, CClipPhysics } from "../../lib/component/CAnimation.js";
import CAniFlow from "../../lib/component/CAniFlow.js";
import CParticle, { CPTCComponent } from "../../lib/component/CPaticle.js";
import CFramework from "../../lib/util/CFramework.js";
import CMath from "../../lib/geometry/CMath.js";
import CWatch from "../../lib/basic/CWatch.js";
import CRenderPass from "../../lib/render/CRenderPass.js";
import Df from "../../lib/basic/Df.js";
import CVec2 from "../../lib/geometry/CVec2.js";
import { CPTCFirework } from "./Firework.js";
import CRigidBody from "../../lib/component/CRigidBody.js";

export default function Flash(_can, _pos) {
	let center = new CObjBD();
	center.SetPos(_pos.toCopy());
	
	let ani = new CAnimation();
	ani.Push(new CClipCObjHD(2000, true));
	center.PushCComponent(new CAniFlow(ani));
	
	//ExplodeFlash(_can, center);
	
	ExplodeCircle(_can, center, new CVec3(1, 0, 1/2), 1000, 1000, 100);
	ExplodeCircle(_can, center, new CVec3(1, 0, -1/2), 1000, 1000, 100);
	
	ExplodeCircle(_can, center, new CVec3(0, 0, 1), 500, 1000, 100);
	ExplodeCircle(_can, center, new CVec3(0, 1, 0), 1500, 1000, 100);
	
	/*
	let smokeColorBefore = new CVec4(0.03,1,0.09,1);
	let smokeColorAfter = new CVec4(0.03,1,0.1, 1);
	ExplodeSmoke(_can, center, new CVec3(1,1,0.57), 100, smokeColorBefore, smokeColorAfter);
	ExplodeSmoke(_can, center, new CVec3(-1,1,0.57), 100, smokeColorBefore, smokeColorAfter);
	ExplodeSmoke(_can, center, new CVec3(0.57,1,-1), 100, smokeColorBefore, smokeColorAfter);
	ExplodeSmoke(_can, center, new CVec3(-0.57,1,-1), 100, smokeColorBefore, smokeColorAfter);
	ExplodeSmoke(_can, center, new CVec3(0,1,1), 100, smokeColorBefore, smokeColorAfter);
	
	let fireColorBefore = new CVec4(0.11, 1, 0.9, 0);
	let fireColorAfter = new CVec4(0.055, 0.9, 0.5, 0);
	ExplodeSmoke(_can, center, new CVec3(1,1,-0.57), 150, fireColorBefore, fireColorAfter);
	ExplodeSmoke(_can, center, new CVec3(-1,1,-0.57), 150, fireColorBefore, fireColorAfter);
	ExplodeSmoke(_can, center, new CVec3(0.57,1,1), 150, fireColorBefore, fireColorAfter);
	ExplodeSmoke(_can, center, new CVec3(-0.57,1,1), 150, fireColorBefore, fireColorAfter);
	ExplodeSmoke(_can, center, new CVec3(0,1,-1), 150, fireColorBefore, fireColorAfter);
	*/
	_can.Push(center);
}

//axis need to be normailzed first
function RotateAroundAxis(vec, axis, radian) {
	let cross = CMath.V3Cross(axis, vec);
	let f = CMath.V3MulFloat(axis, CMath.V3Dot(axis, vec));
	let s = CMath.V3MulFloat(CMath.V3Cross(cross, axis), Math.cos(radian));
	let t = CMath.V3MulFloat(cross, Math.sin(radian));
	
	return CMath.V3AddV3(CMath.V3AddV3(f, s), t);
}

class CPTCSmoke extends CWatch {
	m_dir;
	m_totalObjNum;
	m_totalCycle;
	m_indexObj;
	m_indexCycle;
	m_speed;
	m_lifeMin;
	m_lifeMax;
	m_colorBefore;
	m_colorAfter;
	m_distance;
	m_particlesInOneFrame;
	m_indexParticles;
	m_scale;
	
	constructor() {
        super(...arguments);
        this.m_dir = new CVec3(0, 1, 0);
        this.m_totalObjNum = 30;
        this.m_totalCycle = 3;
        this.m_indexObj = 0;
        this.m_indexCycle = 0;
        this.m_speed = 30;
        this.m_lifeMin = 1000;
        this.m_lifeMax = 1500;
        this.m_colorBefore = new CVec4(0,0,0,0);
        this.m_colorAfter = new CVec4(1,1,1,0);
        this.m_distance = new CVec3(0,0,0);
        this.m_particlesInOneFrame = 1;
        this.m_indexParticles = 0;
        this.m_scale = 0.1;
    }
    Process(_obj, _rb) {
		if(this.m_indexParticles > this.m_particlesInOneFrame) {
			this.m_indexParticles = 0;
		} else {
			this.m_indexParticles++;
		}
		_obj.SetPos(CMath.V3AddV3(_obj.GetPos(), CMath.V3MulFloat(CMath.V3Nor(this.m_dir), this.m_distance / this.m_totalObjNum * this.m_indexParticles)));
		
		let objNumInOneCycle = this.m_totalObjNum / this.m_totalCycle;
		let yaxis = new CVec3(0, 1, 0);
		let vec = CMath.V3Cross(yaxis, this.m_dir);
		let multiplier = (this.m_indexCycle % 2 == 0)? 
			0 + this.m_indexObj / objNumInOneCycle :
			1 - this.m_indexObj / objNumInOneCycle;
		this.m_indexObj++;
		if(this.m_indexObj >= objNumInOneCycle) {
			this.m_indexObj = 0;
			this.m_indexCycle++;
		}
		if(this.m_indexCycle == this.m_totalCycle) {
			this.m_indexObj = 0;
			this.m_indexCycle = 0;
		}
		let angle = multiplier * Math.PI * 2;
		vec = RotateAroundAxis(vec, this.m_dir, angle);
		
		let movement = new CMovement("particle", vec, this.m_speed);
		movement.m_acc = -this.m_speed;
		movement.m_minPow = 5;
		_rb.Push(movement);
		
		let ani = new CAnimation();
		
		let life = Math.random() * (this.m_lifeMax - this.m_lifeMin) + this.m_lifeMin;
		ani.Push(new CClipCObjHD(life, true));
		
		let random = (Math.random() + 1) / 2;
		let mult = 1 - (this.m_indexObj + objNumInOneCycle * this.m_indexCycle) / this.m_totalObjNum;
		let clipDownScale = new CClipPRS(0, life * random, CMath.V3MulFloat(new CVec3(0.5,0.5,0.5), mult * mult * this.m_scale), CMath.V3MulFloat(new CVec3(1,1,1), mult * mult * this.m_scale), 2);
		ani.Push(clipDownScale);
		let clipUpScale = new CClipPRS(life * random, life * (1 - random), CMath.V3MulFloat(new CVec3(1,1,1), mult * mult * this.m_scale), new CVec3(0,0,0), 2);
		clipUpScale.m_curve.m_type = 2;
		ani.Push(clipUpScale);
		
		let clipColor = new CClipColor(0, life, this.m_colorBefore, this.m_colorAfter);
		clipColor.m_space = 2;
		ani.Push(clipColor);
		
		_obj.PushCComponent(new CAniFlow(ani));
    }
}

function ExplodeSmoke(_can, _center, _dir, _distance, _colorBefore, _colorAfter) {
	let obj = new CObjBD();
	
	let ani = new CAnimation();
	ani.Push(new CClipCObjHD(350, true));
	ani.Push(new CClipPRS(0, 350, _center.GetPos().toCopy(), CMath.V3AddV3(_center.GetPos(), CMath.V3MulFloat(_dir, _distance)), 0));
	obj.PushCComponent(new CAniFlow(ani));
	
	_can.Push(obj);
	
	let ptc = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = false;
	rp.m_priority = Df.RenderPriority.Normal + 30;
	rp.m_vf = "PreSimple";
	let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	ptt.SetRenderPass(rp);
	ptcp.m_comArr.push(ptt);
	ptc.m_cptcArr.push(ptcp);
	
	let rb = new CRigidBody();
	rb.Push(new CMovement("gravity", new CVec3(0, -1, 0), 0.98));
	ptcp.m_comArr.push(rb);
	
	let lifeTime = 1200;
	let shape = new CPTCSmoke();
	shape.m_dir = _dir;
	shape.m_totalObjNum = 60;
	shape.m_totalCycle = 3;
	shape.m_speed = 15;
	shape.m_lifeMin = lifeTime * 0.5;
	shape.m_lifeMax = lifeTime * 1.5;
	shape.m_scale = 0.04;
	shape.m_colorBefore = _colorBefore;
	shape.m_colorAfter = _colorAfter;
	shape.m_distance = _distance;
	shape.m_particlesInOneFrame = 3;
	ptc.m_shape = shape;
	
	ptc.m_target = obj;
	ptc.m_createCount = 3;
	ptc.m_createTime = 5;
	ptc.m_endTime = 350;
	
	_can.Push(ptc);
}

function ExplodeFlash(_can, _center) {
	let smallSquare = new CObjBD();
	smallSquare.SetPos(_center.GetPos().toCopy());
	
	let flashBoxSize = new CVec2(15,22.5);
	
	let smallPt = new CPaintBillbord(flashBoxSize, CFramework.Main().Pal().GetBlackTex());
	let smallRP = new CRenderPass();
	smallRP.m_depthTest = false;
	smallRP.m_priority = Df.RenderPriority.Normal + 999;
	smallPt.SetRenderPass(smallRP);
	smallSquare.PushCComponent(smallPt);
	
	let bigSquare = new CObjBD();
	
	let bigPt = new CPaintBillbord(flashBoxSize, CFramework.Main().Pal().GetBlackTex());
	let bigRP = new CRenderPass();
	bigRP.m_depthTest = false;
	bigRP.m_priority = Df.RenderPriority.Normal + 1;
	bigPt.SetRenderPass(bigRP);
	bigSquare.PushCComponent(bigPt);
	smallSquare.PushChilde(bigSquare);
	bigSquare.SetPos(new CVec3(0,0,0));
	
	let time = 300; //소요시간
	
	let smallAni = new CAnimation();
	smallAni.Push(new CClipColor(0, 0, new CVec4(1,1,1,0), new CVec4(1,1,1,0)));
	let clipScaleIncrease = new CClipPRS(0, time * 0.1, new CVec3(1, 1, 1), new CVec3(4, 4, 4), 2);
	clipScaleIncrease.m_curve.m_type = 1;
	smallAni.Push(clipScaleIncrease);
	let clipScaleReduce = new CClipPRS(time * 0.3, time * 0.5, new CVec3(4, 4, 4), new CVec3(0,0, 0), 2);
	clipScaleIncrease.m_curve.m_type = 1;
	smallAni.Push(clipScaleReduce);
	smallAni.Push(new CClipCObjHD(time, true));
	smallSquare.PushCComponent(new CAniFlow(smallAni));
	
	let bigAni = new CAnimation();
	let clipColor = new CClipColor(0, time * 0.2, new CVec4(0.055, 0.3, 0.16, -0.5), new CVec4(0, 1, 0.42, -0.5));
	clipColor.m_space = 2;
	clipColor.m_curve.m_type = 1;
	bigAni.Push(clipColor);
	let clipColor2 = new CClipColor(time * 0.2, time * 0.1, new CVec4(0, 1, 0.42, -0.5), new CVec4(0, 1, 0.42, -1));
	clipColor2.m_space = 2;
	bigAni.Push(clipColor2);
	bigAni.Push(new CClipCObjHD(time * 0.3, true));
	let clipScale = new CClipPRS(0, time * 0.2, new CVec3(1.2, 1.2, 1.2), new CVec3(2,2,2), 2);
	clipScale.m_curve.m_type = 1;
	bigAni.Push(clipScale);
	bigSquare.PushCComponent(new CAniFlow(bigAni));
	
	_can.Push(smallSquare);
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

export function ExplodeCircle(_can, _obj, _axis, _radius, _life, _num) {
	let Particle = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = false;
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
	shape.m_colorMin = _colorMin;
	shape.m_colorMax = _colorMax;
	shape.m_scaleMax = 0.04;
	shape.m_scaleMin = 0.02;
	shape.m_axis = _axis;
	shape.m_count = _num;
	
	Particle.m_shape = shape;
	Particle.m_createCount = _num;
	Particle.m_createTime = 100;
	Particle.m_endTime = 100;
	Particle.m_startTime = _startTime;
	Particle.m_target = _obj;
	
	_can.Push(Particle);
}